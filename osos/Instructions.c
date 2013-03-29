#include "Instructions.h"
#include "OpCodes.h"

#define READ_BYTE(p) (*((byte*) p))
#define READ_WORD(p) (*((word*) p))
#define READ_UWORD(p) (*((uword*) p))

word exec_instruction(struct VirtualMachine *vm, byte opcode, void *operands)
{
    return get_instruction(opcode)(vm, operands);
}

word Op_nop(struct VirtualMachine *vm, void *operands)
{
    return sizeof(op);
}

word Op_exit(struct VirtualMachine *vm, void *operands)
{
    VM_exit(vm);
    return sizeof(op);
}

word Op_push_constant(struct VirtualMachine *vm, void *operands)
{
    VM_push(vm, READ_WORD(operands));
    return sizeof(op) + sizeof(word);
}

word Op_push_bp(struct VirtualMachine *vm, void *operands)
{
    VM_push(vm, VM_get_bp(vm));
    return sizeof(op);
}

word Op_push_sp(struct VirtualMachine *vm, void *operands)
{
    VM_push(vm, VM_get_sp(vm));
    return sizeof(op);
}

word Op_push_hp(struct VirtualMachine *vm, void *operands)
{
    VM_push(vm, VM_get_hp(vm));
    return sizeof(op);
}

word Op_pop(struct VirtualMachine *vm, void *operands)
{
    VM_pop(vm);
    return sizeof(op);
}

word Op_pop_bp(struct VirtualMachine *vm, void *operands)
{
    VM_set_bp(vm, VM_pop(vm));
    return sizeof(op);
}

word Op_pop_sp(struct VirtualMachine *vm, void *operands)
{
    VM_set_sp(vm, VM_pop(vm));
    return sizeof(op);
}

word Op_pop_hp(struct VirtualMachine *vm, void *operands)
{
    VM_set_hp(vm, VM_pop(vm));
    return sizeof(op);
}

word Op_duplicate(struct VirtualMachine *vm, void *operands)
{
    word w = VM_load(vm, 0);
    VM_push(vm, w);
    return sizeof(op);
}

word Op_duplicate2(struct VirtualMachine *vm, void *operands)
{
    word w = VM_load(vm, 1);
    VM_push(vm, w);
    return sizeof(op);
}

word Op_swap(struct VirtualMachine *vm, void *operands)
{
    word w1 = VM_pop(vm);
    word w2 = VM_pop(vm);
    VM_push(vm, w1);
    VM_push(vm, w2);
    return sizeof(op);
}

word Op_copy(struct VirtualMachine *vm, void *operands)
{
    VM_push(vm, VM_read(vm));
    return sizeof(op);
}

word Op_move(struct VirtualMachine *vm, void *operands)
{
    VM_write(vm, VM_pop(vm));
    return sizeof(op);
}

word Op_jump(struct VirtualMachine *vm, void *operands)
{
    VM_jump(vm, READ_WORD(operands));
    return 0;
}

word Op_jump_equal(struct VirtualMachine *vm, void *operands)
{
    word w1 = VM_load(vm, 0);
    word w2 = VM_load(vm, 1);
    if (w1 == w2) {
        VM_jump(vm, READ_WORD(operands));
        return 0;
    } else
        return sizeof(op) + sizeof(word);
}

word Op_jump_not_equal(struct VirtualMachine *vm, void *operands)
{
    word w1 = VM_load(vm, 0);
    word w2 = VM_load(vm, 1);
    if (w1 != w2) {
        VM_jump(vm, READ_WORD(operands));
        return 0;
    } else
        return sizeof(op) + sizeof(word);
}

word Op_jump_greater(struct VirtualMachine *vm, void *operands)
{
    word w1 = VM_load(vm, 0);
    word w2 = VM_load(vm, 1);
    if (w1 > w2) {
        VM_jump(vm, READ_WORD(operands));
        return 0;
    } else
        return sizeof(op) + sizeof(word);
}

word Op_jump_greater_equal(struct VirtualMachine *vm, void *operands)
{
    word w1 = VM_load(vm, 0);
    word w2 = VM_load(vm, 1);
    if (w1 >= w2) {
        VM_jump(vm, READ_WORD(operands));
        return 0;
    } else
        return sizeof(op) + sizeof(word);
}

word Op_jump_less(struct VirtualMachine *vm, void *operands)
{
    word w1 = VM_load(vm, 0);
    word w2 = VM_load(vm, 1);
    if (w1 < w2) {
        VM_jump(vm, READ_WORD(operands));
        return 0;
    } else
        return sizeof(op) + sizeof(word);
}

word Op_jump_less_equal(struct VirtualMachine *vm, void *operands)
{
    word w1 = VM_load(vm, 0);
    word w2 = VM_load(vm, 1);
    if (w1 <= w2) {
        VM_jump(vm, READ_WORD(operands));
        return 0;
    } else
        return sizeof(op) + sizeof(word);
}

word Op_add(struct VirtualMachine *vm, void *operands)
{
    word w1 = VM_pop(vm);
    word w2 = VM_pop(vm);
    word w = w1 + w2;
    VM_push(vm, w);
    return sizeof(op);
}

word Op_subtract(struct VirtualMachine *vm, void *operands)
{
    word w1 = VM_pop(vm);
    word w2 = VM_pop(vm);
    word w = w1 - w2;
    VM_push(vm, w);
    return sizeof(op);
}

word Op_multiply(struct VirtualMachine *vm, void *operands)
{
    word w1 = VM_pop(vm);
    word w2 = VM_pop(vm);
    word w = w1 * w2;
    VM_push(vm, w);
    return sizeof(op);
}

word Op_divide(struct VirtualMachine *vm, void *operands)
{
    word w1 = VM_pop(vm);
    word w2 = VM_pop(vm);
    word w = w1 / w2;
    VM_push(vm, w);
    return sizeof(op);
}

word Op_bitwise_and(struct VirtualMachine *vm, void *operands)
{
    word w1 = VM_pop(vm);
    word w2 = VM_pop(vm);
    word w = w1 & w2;
    VM_push(vm, w);
    return sizeof(op);
}

word Op_bitwise_or(struct VirtualMachine *vm, void *operands)
{
    word w1 = VM_pop(vm);
    word w2 = VM_pop(vm);
    word w = w1 | w2;
    VM_push(vm, w);
    return sizeof(op);
}

word Op_bitwise_not(struct VirtualMachine *vm, void *operands)
{
    word w = ~VM_pop(vm);
    VM_push(vm, w);
    return sizeof(op);
}

word Op_bitwise_xor(struct VirtualMachine *vm, void *operands)
{
    word w1 = VM_pop(vm);
    word w2 = VM_pop(vm);
    word w = w1 ^ w2;
    VM_push(vm, w);
    return sizeof(op);
}

word Op_logical_and(struct VirtualMachine *vm, void *operands)
{
    word w1 = VM_pop(vm);
    word w2 = VM_pop(vm);
    word w = w1 && w2;
    VM_push(vm, w);
    return sizeof(op);
}

word Op_logical_or(struct VirtualMachine *vm, void *operands)
{
    word w1 = VM_pop(vm);
    word w2 = VM_pop(vm);
    word w = w1 || w2;
    VM_push(vm, w);
    return sizeof(op);
}

word Op_logical_not(struct VirtualMachine *vm, void *operands)
{
    word w = !VM_pop(vm);
    VM_push(vm, w);
    return sizeof(op);
}
