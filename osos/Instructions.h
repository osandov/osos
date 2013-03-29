#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

/**
 * @file Instructions.h
 * Implementation of instructions
 */

#include "osos/VirtualMachine.h"

typedef word (*Instruction)(struct VirtualMachine *vm, void *operands);

word exec_instruction(struct VirtualMachine *vm, op opcode, void *operands);

word Op_nop(struct VirtualMachine *vm, void *operands);
word Op_exit(struct VirtualMachine *vm, void *operands);

word Op_push_constant(struct VirtualMachine *vm, void *operands);
word Op_push_bp(struct VirtualMachine *vm, void *operands);
word Op_push_sp(struct VirtualMachine *vm, void *operands);
word Op_push_hp(struct VirtualMachine *vm, void *operands);

word Op_pop(struct VirtualMachine *vm, void *operands);
word Op_pop_bp(struct VirtualMachine *vm, void *operands);
word Op_pop_sp(struct VirtualMachine *vm, void *operands);
word Op_pop_hp(struct VirtualMachine *vm, void *operands);

word Op_duplicate(struct VirtualMachine *vm, void *operands);
word Op_duplicate2(struct VirtualMachine *vm, void *operands);
word Op_swap(struct VirtualMachine *vm, void *operands);

word Op_copy(struct VirtualMachine *vm, void *operands);
word Op_move(struct VirtualMachine *vm, void *operands);

word Op_jump(struct VirtualMachine *vm, void *operands);
word Op_jump_equal(struct VirtualMachine *vm, void *operands);
word Op_jump_not_equal(struct VirtualMachine *vm, void *operands);
word Op_jump_greater(struct VirtualMachine *vm, void *operands);
word Op_jump_greater_equal(struct VirtualMachine *vm, void *operands);
word Op_jump_less(struct VirtualMachine *vm, void *operands);
word Op_jump_less_equal(struct VirtualMachine *vm, void *operands);

word Op_add(struct VirtualMachine *vm, void *operands);
word Op_subtract(struct VirtualMachine *vm, void *operands);
word Op_multiply(struct VirtualMachine *vm, void *operands);
word Op_divide(struct VirtualMachine *vm, void *operands);

word Op_and(struct VirtualMachine *vm, void *operands);
word Op_subtract(struct VirtualMachine *vm, void *operands);
word Op_multiply(struct VirtualMachine *vm, void *operands);
word Op_divide(struct VirtualMachine *vm, void *operands);

word Op_bitwise_and(struct VirtualMachine *vm, void *operands);
word Op_bitwise_or(struct VirtualMachine *vm, void *operands);
word Op_bitwise_not(struct VirtualMachine *vm, void *operands);
word Op_bitwise_xor(struct VirtualMachine *vm, void *operands);

word Op_logical_and(struct VirtualMachine *vm, void *operands);
word Op_logical_or(struct VirtualMachine *vm, void *operands);
word Op_logical_not(struct VirtualMachine *vm, void *operands);

#endif /* INSTRUCTIONS_H */
