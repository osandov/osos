#include "Assemble.h"
#include "Buffer.h"
#include "Error.h"
#include "Lexer.h"
#include "Output.h"
#include "Passes.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>

int assemble(char **input_files, int filen, char *output_file)
{
    struct Buffer *buffer;
    struct Line *lines;
    struct OpList *ops;
    struct LabelTable *labels;
    int r = 0;

    buffer = buffer_files(input_files, filen);
    if (buffer) {
        lines = lex(buffer);
        free_buffer(buffer);

        if (lines) {
            labels = create_label_table();
            r |= first_pass(lines, &ops, labels);
            r |= second_pass(ops, labels);

            if (!r) {
                FILE *f = fopen(output_file, "wb");
                if (!f) { 
                    error("%s: %s\n", output_file, strerror(errno));
                    r = 1;
                } else {
                    r = write_output(f, ops);
                    fclose(f);
                }
            }
            free_oplist(ops);
            free_label_table(labels);
            free_lines(lines);
        } else
            return 1;
    } else
        return 1;
    return r;
}
