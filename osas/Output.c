#include "Output.h"
#include "Error.h"

int write_output(FILE *file, struct OpList *ops)
{
    while (ops) {
        fwrite(&(ops->opcode.opcode), sizeof(op), 1, file);
        switch (ops->opcode.operand.type) {
            case OPERAND_NUMBER:
                fwrite(&(ops->opcode.operand.num), sizeof(word), 1, file);
                break;
            case OPERAND_NONE:
            case OPERAND_REG:
                break;
            default:
                error("An invalid opcode was generated\n");
                return 1;
        }
        ops = ops->next;
    }
    return 0;
}
