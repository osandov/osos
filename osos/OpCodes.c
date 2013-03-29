#include "OpCodes.h"
#include <stdio.h>
#include <string.h>

/**
 * A definition for an operation, used for translating between a mnemonic and
 * and opcode
 */
struct OpDef {
    /** The mnemonic for the operation */
    const char *mnemonic;

    /** The instruction corresponding to the operation */
    Instruction instruction;

    /** The type of operand taken by the operation */
    enum OperandType operand;

    /** The size of the operation */
    int size;
};

const struct OpDef opdefs[] = {
    {"nop",  Op_nop,           OPERAND_NONE,        sizeof(op)},
    {"exit", Op_exit,          OPERAND_NONE,        sizeof(op)},

    {"push", Op_push_constant, OPERAND_NUM_OR_REG,  sizeof(op) + sizeof(word)},
    {"push", Op_push_bp,       OPERAND_REG,         sizeof(op)},
    {"push", Op_push_sp,       OPERAND_REG,         sizeof(op)},
    {"push", Op_push_hp,       OPERAND_REG,         sizeof(op)},

    {"pop",  Op_pop,           OPERAND_NONE_OR_REG, sizeof(op)},
    {"pop",  Op_pop_bp,        OPERAND_REG,         sizeof(op)},
    {"pop",  Op_pop_sp,        OPERAND_REG,         sizeof(op)},
    {"pop",  Op_pop_hp,        OPERAND_REG,         sizeof(op)},

    {"dup",  Op_duplicate,     OPERAND_NONE,        sizeof(op)},
    {"dup2", Op_duplicate2,    OPERAND_NONE,        sizeof(op)},
    {"swap", Op_swap,          OPERAND_NONE,        sizeof(op)},

    {"copy", Op_copy,          OPERAND_NONE,        sizeof(op)},
    {"mov",  Op_move,          OPERAND_NONE,        sizeof(op)},

    {"jmp",  Op_jump,               OPERAND_LABEL,  sizeof(op) + sizeof(word)},
    {"je",   Op_jump_equal,         OPERAND_LABEL,  sizeof(op) + sizeof(word)},
    {"jne",  Op_jump_not_equal,     OPERAND_LABEL,  sizeof(op) + sizeof(word)},
    {"jg",   Op_jump_greater,       OPERAND_LABEL,  sizeof(op) + sizeof(word)},
    {"jge",  Op_jump_greater_equal, OPERAND_LABEL,  sizeof(op) + sizeof(word)},
    {"jl",   Op_jump_less,          OPERAND_LABEL,  sizeof(op) + sizeof(word)},
    {"jle",  Op_jump_less_equal,    OPERAND_LABEL,  sizeof(op) + sizeof(word)},

    {"add",  Op_add,           OPERAND_NONE,        sizeof(op)},
    {"sub",  Op_subtract,      OPERAND_NONE,        sizeof(op)},
    {"mul",  Op_multiply,      OPERAND_NONE,        sizeof(op)},
    {"div",  Op_divide,        OPERAND_NONE,        sizeof(op)},

    {"and",   Op_bitwise_and,  OPERAND_NONE,        sizeof(op)},
    {"or",    Op_bitwise_or,   OPERAND_NONE,        sizeof(op)},
    {"not",   Op_bitwise_not,  OPERAND_NONE,        sizeof(op)},
    {"xor",   Op_bitwise_xor,  OPERAND_NONE,        sizeof(op)},

    {"land",  Op_logical_and,  OPERAND_NONE,        sizeof(op)},
    {"lor",   Op_logical_or,   OPERAND_NONE,        sizeof(op)},
    {"lnot",  Op_logical_not,  OPERAND_NONE,        sizeof(op)}
};

const size_t num_ops = sizeof(opdefs) / sizeof(struct OpDef);

static char *read_label(const char *str);

Instruction get_instruction(op opcode)
{
    return opdefs[opcode].instruction;
}

enum TranslateError mne_to_op(const char *mnemonic, const char *operand,
                              struct OpCode *opcode)
{
    size_t i;

    if (!operand)
        operand = "";

    for (i = 0; i < num_ops; ++i) {
        if (!strcmp(mnemonic, opdefs[i].mnemonic)) {
            /* Super weird logic starts here */
            switch ((opcode->operand.type = opdefs[i].operand)) {
                case OPERAND_NONE_OR_REG:
                    if (!operand[0]) {
                        opcode->operand.type = OPERAND_NONE;
                        break;
                    } else if (operand[0] != '%')
                        return TRANS_WRONG_OPERAND;
                    /* Intentional fall-through */
                case OPERAND_NUM_OR_REG:
                    if (operand[0] == '%') {
                        opcode->operand.type = OPERAND_REG;
                        if (!strcmp(operand, "%bp"))
                            i += 1;
                        else if (!strcmp(operand, "%sp"))
                            i += 2;
                        else if (!strcmp(operand, "%hp"))
                            i += 3;
                        else
                            return TRANS_UNREC_REG;
                        break;
                    }
                    /* Intentional fall-through */
                case OPERAND_NUMBER:
                    if (!sscanf(operand, "%i", &(opcode->operand.num)))
                        return TRANS_WRONG_OPERAND;
                    else
                        opcode->operand.type = OPERAND_NUMBER;
                    break;
                case OPERAND_LABEL:
                    if (!sscanf(operand, "%i", &(opcode->operand.num))) {
                        if (!(opcode->operand.label = read_label(operand)))
                            return TRANS_WRONG_OPERAND;
                    } else
                        opcode->operand.type = OPERAND_NUMBER;
                    break;
                case OPERAND_NONE:
                    if (operand[0])
                        return TRANS_WRONG_OPERAND;
                    break;
                default:
                    break;
            }
            opcode->opcode = i;
            opcode->size = opdefs[i].size;
            return TRANS_SUCCESS;
        }
    }
    return TRANS_UNREC_MNE;
}

const char *op_to_mne(struct OpCode *opcode)
{
    int i = opcode->opcode;
    opcode->operand.type = opdefs[i].operand;
    opcode->size = opdefs[i].size;
    return opdefs[i].mnemonic;
}

static char *read_label(const char *str)
{
    int n = 0, i;
    char *label;
    while (str[n])
        ++n;
    if (!n)
        return NULL;
    ++n;
    label = (char*) malloc(sizeof(char) * n);
    for (i = 0; i < n; ++i)
        label[i] = str[i];
    return label;
}
