#include "base64.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static void usage(void) {
    fprintf(stderr,
        "usage:\n"
        "  base64 -e input [output] [-c comment]\n"
        "  base64 -d input [output]\n");
} // на всякий случай

int main(int argc, char **argv) {
    if (argc < 3) {
        usage();
        return 1;
    }

    int encode = 0;
    if (strcmp(argv[1], "-e") == 0)
        encode = 1;
    else if (strcmp(argv[1], "-d") == 0)
        encode = 0;
    else {
        usage();
        return 1;
    }

    FILE *in = fopen(argv[2], encode ? "rb" : "r"); // везде b игнорится для линукса и макоса, но для винды самое то. хотя пофиг на винду, потому что у меня уже до этого несовместимо с виндой
    if (!in) { perror("input"); return 1; }

    FILE *out;
    char outname[1024];
    const char *comment = NULL;
    int arg_index = 3;


    if (arg_index < argc && argv[arg_index][0] != '-') {
        strncpy(outname, argv[arg_index], sizeof(outname)-1);
        outname[sizeof(outname)-1] = 0;
        out = fopen(outname, encode ? "w" : "wb");
        if (!out) { perror("output"); fclose(in); return 1; }
        arg_index++;
    } else {
        strcpy(outname, argv[2]);
        if (encode) strcat(outname, ".base64"); else strcat(outname, ".out");
        out = fopen(outname, encode ? "w" : "wb");
        if (!out) { perror("output"); fclose(in); return 1; }
    }


    while (arg_index + 1 < argc) {
        if (strcmp(argv[arg_index], "-c") == 0) {
            comment = argv[arg_index + 1];
            break;
        }
        arg_index++;
    }

    if (encode) {
        base64_encode(in, out, comment);
    } else {
        base64_init();
        base64_decode(in, out);
    }

    fclose(in);
    fclose(out);
    return 0;
}
