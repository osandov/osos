#include <stdio.h>
#include "VirtualMachine.h"
#include "Instructions.h"

/**
 * A virtual machine
 */
struct VirtualMachine {
    /** The virtual machine's buffer, containing the instructions and stack */
    void *buffer;

    /** The size of the buffer */
    size_t buffer_size;

    /** The beginning of the instruction section */
    char *inst;

    /** The current instruction */
    char *pc;

    /** The bottom of the stack in the buffer */
    word *base;

    /** The base pointer, pointing to the bottom of the current stack frame */
    word *bp;

    /** The stack pointer, pointing to the top of the current stack frame */
    word *sp;

    /**
     * The head pointer, used for reading from and writing to arbitrary
     * locations in memory
     */
    word *hp;

    /** Whether the virtual machine is still active */
    int active;

    /** The exit status set by an exit instruction */
    int exit_status;
};

struct VirtualMachine *VM_create(size_t size)
{
    struct VirtualMachine *vm =
        (struct VirtualMachine*) malloc(sizeof(struct VirtualMachine));
    vm->buffer = malloc(size);
    vm->buffer_size = size;

    vm->inst = (char*) vm->buffer;
    vm->pc = vm->inst;

    vm->base = (word*) ((char*) vm->buffer + size / 2);
    vm->hp = vm->sp = vm->bp = vm->base;

    vm->active = 1;
    vm->exit_status = 0;
    return vm;
}

void VM_destroy(struct VirtualMachine *vm)
{
    free(vm->buffer);
    free(vm);
}

void *VM_get_instructions(struct VirtualMachine *vm)
{
    return vm->inst;
}

int VM_execute(struct VirtualMachine *vm)
{
    do {
        char opcode = *vm->pc;
        word offset = exec_instruction(vm, opcode, vm->pc + sizeof(op));
        vm->pc += offset;
    } while (vm->active);
    return vm->exit_status;
}

void VM_exit(struct VirtualMachine *vm)
{
    vm->active = 0;
    vm->exit_status = VM_pop(vm);
}

void VM_jump(struct VirtualMachine *vm, uword offset)
{
    vm->pc = vm->inst + offset;
}

void VM_push(struct VirtualMachine *vm, word data)
{
    *vm->sp = data;
    ++vm->sp;
}

word VM_pop(struct VirtualMachine *vm)
{
    --vm->sp;
    return *vm->sp;
}

word VM_load(struct VirtualMachine *vm, word offset)
{
    return *(vm->sp - (offset + 1));
}

word VM_read(struct VirtualMachine *vm)
{
    return *vm->hp;
}

void VM_write(struct VirtualMachine *vm, word data)
{
    *vm->hp = data;
}

uword VM_get_bp(struct VirtualMachine *vm)
{
    return (word) (vm->bp - vm->base);
}

uword VM_get_sp(struct VirtualMachine *vm)
{
    return (word) (vm->sp - vm->base);
}

uword VM_get_hp(struct VirtualMachine *vm)
{
    return (word) (vm->hp - vm->base);
}

void VM_set_bp(struct VirtualMachine *vm, uword offset)
{
    vm->bp = vm->base + offset;
}

void VM_set_sp(struct VirtualMachine *vm, uword offset)
{
    vm->sp = (word*) vm->base + offset;
}

void VM_set_hp(struct VirtualMachine *vm, uword offset)
{
    vm->hp = vm->base + offset;
}

void VM_print_stack(struct VirtualMachine *vm)
{
    word address = 0x0;
    word *sp = vm->bp;
    while (sp != vm->sp) {
        printf("0x%x:\t0x%x", address, *sp);
        if (sp == vm->bp)
            printf(" (bp)");
        if (sp == vm->hp)
            printf(" (hp)");
        printf("\n");
        address++;
        ++sp;
    }
    printf("0x%x:\tsp\n", address);
    if (!vm->active) {
        if (vm->exit_status)
            printf("Exited with code 0x%x\n", vm->exit_status);
        else
            printf("Exited normally\n");
    }
}
