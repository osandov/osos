#include "Assemble.h"
#include "Error.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

static char *output_file = "out.ov";

int read_opts(int argc, char **argv)
{
    static struct option longopts[] = {
        {"help", no_argument, NULL, 0},
        {NULL, 0, NULL, 0}
    };

    static const char *usage_string = "Usage: %s [-o outfile] infile...\n";

    int c, option_index;

    while ((c = getopt_long(argc, argv, "o:", longopts, &option_index)) != -1) {
        switch (c) {
            case 0:
                printf(usage_string, progname);
                exit(EXIT_SUCCESS);
                break;
            case 'o':
                output_file = optarg;
                break;
            default: /* '?' */
                fprintf(stderr, usage_string, progname);
                exit(EXIT_FAILURE);
        }
    }

    if (!(argc - optind)) {
        fprintf(stderr, "%s: no input files\n", progname);
        exit(EXIT_FAILURE);
    }

    return optind;
}

int main(int argc, char **argv)
{
    int optind;
    progname = argv[0];
    optind = read_opts(argc, argv);
    return assemble(argv + optind, argc - optind, output_file);
}
