#include "osos/VirtualMachine.h"
#include <stdlib.h>
#include <stdio.h>

struct VirtualMachine *vm = NULL;

/* 128 MB */
size_t buffer_size = 134217728;

/** Reads the bytecode contained in the given file into the given buffer
 * @return Non-zero if the bytecode is too big to fit in the buffer
 */
int read_bytecode(FILE *file, void *buffer)
{
    return fread(buffer, 1, buffer_size / 2, file) == buffer_size / 2;
}

int main(int argc, const char *argv[])
{
    int ret = 0;
    if (argc > 2) {
        fprintf(stderr, "USAGE: %s [FILE]\n", argv[0]);
        return 1;
    }

    vm = VM_create(buffer_size);

    if (argc == 2) {
        FILE *file = fopen(argv[1], "r");
        if (!file) {
            fprintf(stderr, "%s: %s: No such file or directory\n",
                    argv[0], argv[1]);
            ret = 1;
            goto done;
        }
        read_bytecode(file, VM_get_instructions(vm));
        fclose(file);
    } else if (argc == 1) {
        read_bytecode(stdin, VM_get_instructions(vm));
    }

    if (!vm) {
        fprintf(stderr, "%s: Out of memory\n", argv[0]);
        ret = 1;
        goto done;
    }

    ret = VM_execute(vm);
    VM_print_stack(vm);

done:
    VM_destroy(vm);
    return ret;
}
