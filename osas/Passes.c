#include "Passes.h"
#include "Error.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

enum LabelRead {
    LABEL_VALID,
    LABEL_NO_MATCH,
    LABEL_EMPTY,
    LABEL_TRAILING,
    LABEL_DIGIT,
    LABEL_NON_ALNUM
};

static enum LabelRead read_label(const char *in, char **out);

static void wrong_operand(struct Line *line, struct OpCode *op);

int first_pass(struct Line *lines, struct OpList **out_ops,
               struct Label **out_labels)
{
    struct OpList *op_front = NULL, *ops;
    struct Label *labels = NULL;
    uword address = 0x0;
    int err = 0;

    while (lines->tokens) {
        char *label_name;
        int is_label = 0;
        switch (read_label(lines->tokens->token, &label_name)) {
            case LABEL_VALID:
                if (lines->tokens->next) {
                    struct Line *temp =
                        (struct Line*) malloc(sizeof(struct Line));
                    memcpy(temp, lines, sizeof(struct Line));
                    temp->tokens = temp->tokens->next;
                    lines->tokens->next = NULL;
                    lines->next = temp;
                }
                is_label = 1;
                break;
            case LABEL_NO_MATCH:
                break;
            case LABEL_EMPTY:
                compile_error(lines->filename, lines->lineno,
                        "Empty label\n");
                err = 1;
                is_label = 2;
                break;
            case LABEL_TRAILING:
                compile_error(lines->filename, lines->lineno,
                        "Trailing characters after ':'\n");
                err = 1;
                is_label = 2;
                break;
            case LABEL_DIGIT:
                compile_error(lines->filename, lines->lineno,
                        "Label begins with a digit\n");
                err = 1;
                is_label = 2;
                break;
            case LABEL_NON_ALNUM:
                compile_error(lines->filename, lines->lineno,
                        "Label contains non-alphanumeric character\n");
                err = 1;
                is_label = 2;
                break;
        }
        if (is_label == 1) {
            struct Label *label =
                (struct Label*) calloc(1, sizeof(struct Label));
            label->name = label_name;
            labels = add_label(labels, label);
            if (label->address) {
                compile_error(lines->filename, lines->lineno,
                        "Label '%s' has already been defined\n", label_name);
                err = 1;
            } else
                label->address = address;
        } else if (!is_label) {
            struct OpCode opcode;
            const char *mnemonic = lines->tokens->token;
            const char *operand = lines->tokens->next ?
                lines->tokens->next->token : "";
            struct OpList *op;
            switch (mne_to_op(mnemonic, operand, &opcode)) {
                case TRANS_SUCCESS:
                    if (lines->tokens->next && lines->tokens->next->next) {
                        wrong_operand(lines, &opcode);
                        err = 1;
                    } else {
                        op = (struct OpList*) malloc(sizeof(struct OpList));
                        address += opcode.size;
                        memcpy(&op->opcode, &opcode, sizeof(struct OpCode));
                        op->line = lines;
                        op->next = NULL;

                        if (op_front)
                            ops = ops->next = op;
                        else
                            ops = op_front = op;
                    }
                    break;
                case TRANS_UNREC_MNE:
                    compile_error(lines->filename, lines->lineno,
                            "Unrecognized mnemonic '%s'\n",
                            lines->tokens->token);
                    err = 1;
                    break;
                case TRANS_UNREC_REG:
                    compile_error(lines->filename, lines->lineno,
                            "Unrecognized register '%s'\n", operand);
                    err = 1;
                    break;
                case TRANS_WRONG_OPERAND:
                    wrong_operand(lines, &opcode);
                    err = 1;
                    break;
            }
        }
        lines = lines->next;
    }

    *out_ops = op_front;
    *out_labels = labels;
    return err;
}

int second_pass(struct OpList *ops, struct Label *labels)
{
    struct OpList *op = ops;
    int err = 0;
    while (op) {
        if (op->opcode.operand.type == OPERAND_LABEL) {
            char *label_name = op->opcode.operand.label;
            struct Label *label = get_label(labels, label_name);
            if (label) {
                free(label_name);
                op->opcode.operand.type = OPERAND_NUMBER;
                op->opcode.operand.num = label->address;
            } else {
                compile_error(op->line->filename, op->line->lineno,
                        "Unrecognized label '%s'\n", label_name);
                err = 1;
            }
        }
        op = op->next;
    }
    return err;
}

void free_oplist(struct OpList *front)
{
    if (front) {
        free_oplist(front->next);
        free(front);
    }
}

static enum LabelRead read_label(const char *in, char **out)
{
    const char *p = in;
    char* r;
    size_t n = 0;

    do {
        if (!*p)
            return LABEL_NO_MATCH;
        else if (*p == ':')
            break;
        else if (!isalnum(*p) && *p != '_')
            return LABEL_NON_ALNUM;
        ++p;
        ++n;
    } while (1);

    if (!n)
        return LABEL_EMPTY;
    else if (*(p + 1))
        return LABEL_TRAILING;
    else if (isdigit(*in))
        return LABEL_DIGIT;

    r = (char*) malloc(n + 1);
    memcpy(r, in, n);
    r[n] = '\0';
    *out = r;
    return LABEL_VALID;
}

static void wrong_operand(struct Line *line, struct OpCode *opcode)
{
    switch (opcode->operand.type) {
        case OPERAND_NONE:
            compile_error(line->filename, line->lineno,
                    "'%s' does not take any operands\n", line->tokens->token);
            break;
        case OPERAND_NUMBER:
            compile_error(line->filename, line->lineno,
                    "'%s' takes one numerical operand\n", line->tokens->token);
            break;
        case OPERAND_REG:
            compile_error(line->filename, line->lineno,
                    "'%s' takes one register operand\n", line->tokens->token);
            break;
        case OPERAND_LABEL:
            compile_error(line->filename, line->lineno,
                    "'%s' takes one label or constant offset as an operand\n",
                    line->tokens->token);
            break;
        case OPERAND_NUM_OR_REG:
            compile_error(line->filename, line->lineno,
                    "'%s' takes one constant or register as an operand\n",
                    line->tokens->token);
            break;
        case OPERAND_NONE_OR_REG:
            compile_error(line->filename, line->lineno,
                    "'%s' takes one register or no operands\n",
                    line->tokens->token);
            break;
    }
}
