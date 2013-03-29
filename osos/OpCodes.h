#ifndef OPCODES_H
#define OPCODES_H

/**
 * @file OpCodes.h
 * The definitions of opcodes and opcode lookup functions
 */

#include "osos/VirtualMachine.h"
#include "osos/Instructions.h"

/**
 * The type of operand taken by an operation
 */
enum OperandType {
    OPERAND_NONE,
    OPERAND_NUMBER,
    OPERAND_REG,
    OPERAND_LABEL,
    OPERAND_NUM_OR_REG,
    OPERAND_NONE_OR_REG
};

/**
 * An operand taken by an operation
 */
struct Operand {
    /** The type of operand */
    enum OperandType type;

    union {
        /** A numeric operand */
        word num;

        /** A label operand, used by jump operations */
        char *label;
    };
};

/**
 * An operation, consisting of a bytecode value and any operands
 */
struct OpCode {
    /** The bytecode value of the operation */
    op opcode;

    /** Information about any operands this operation requires */
    struct Operand operand;

    /** The size of the operation, including any operands */
    int size;
};

/**
 * Status returned when translating between a mnemonic and an operand
 */
enum TranslateError {
    TRANS_SUCCESS,
    TRANS_UNREC_MNE,
    TRANS_UNREC_REG,
    TRANS_WRONG_OPERAND
};

/**
 * Return the instruction corresponding to the given opcode
 * @note Does not check for validity.
 */
Instruction get_instruction(op opcode);

/**
 * Look up the given the mnemonic
 * @param [in] mnemonic The mnemonic to search for
 * @param [in] operand The operand string
 * @param [out] opcode An OpCode struct to fill in
 * @return Zero on success, non-zero on an error.
 * @note If the operand is a label, a string will be allocated and must be freed
 * by the caller
 */
enum TranslateError mne_to_op(const char *mnemonic, const char *operand,
                              struct OpCode *opcode);

/**
 * Look up the given opcode
 */
const char *op_to_mne(struct OpCode *opcode);

#endif /* OPCODES_H */
