#ifndef VIRTUALMACHINE_H
#define VIRTUALMACHINE_H

/**
 * @file VirtualMachine.h
 * The virtual machine interface
 */

#include <stdlib.h>
#include "osos/defs.h"

struct VirtualMachine;

/**
 * Creates a new virtual machine
 * @param size The number of bytes to reserve for the instructions and stack
 */
struct VirtualMachine *VM_create(size_t size);

/**
 * Destroys the virtual machine and its buffer
 */
void VM_destroy(struct VirtualMachine *vm);

/**
 * Returns a pointer to the instructions in memory
 */
void *VM_get_instructions(struct VirtualMachine *vm);

/**
 * Executes the program loaded in the virtual machine
 * @return The exit status
 */
int VM_execute(struct VirtualMachine *vm);

/**
 * Sets the exit status of the program to the value on top of the stack and
 * sets the exit flag
 */
void VM_exit(struct VirtualMachine *vm);

/**
 * Jumps to the instruction as an offset in bytes from the first instruction
 */
void VM_jump(struct VirtualMachine *vm, uword offset);

/**
 * Pushes a word onto the stack
 */
void VM_push(struct VirtualMachine *vm, word data);

/**
 * Pops a word off of the stack and returns it
 */
word VM_pop(struct VirtualMachine *vm);

/**
 * Returns a word from the stack without modifying state
 * @param offset An offset, in bytes, from the top of the stack. Zero refers to
 * the word on top oft he stack
 */
word VM_load(struct VirtualMachine *vm, word offset);

/**
 * Returns the word located at the head
 */
word VM_read(struct VirtualMachine *vm);

/**
 * Sets the word located at the head
 */
void VM_write(struct VirtualMachine *vm, word data);

/**
 * Returns the offset in words of the base pointer from the bottom of the stack
 */
uword VM_get_bp(struct VirtualMachine *vm);

/**
 * Returns the offset in words of the stack pointer from the bottom of the stack
 */
uword VM_get_sp(struct VirtualMachine *vm);

/**
 * Returns the offset in words of the head pointer from the bottom of the stack
 */
uword VM_get_hp(struct VirtualMachine *vm);

/**
 * Sets the base pointer as an offset in words from the bottom of the stack
 */
void VM_set_bp(struct VirtualMachine *vm, uword offset);

/**
 * Sets the stack pointer as an offset in words from the bottom of the stack
 */
void VM_set_sp(struct VirtualMachine *vm, uword offset);

/**
 * Sets the head pointer as an offset in words from the bottom of the stack
 */
void VM_set_hp(struct VirtualMachine *vm, uword offset);

void VM_print_stack(struct VirtualMachine *vm);

#endif /* VIRTUALMACHINE_H */
