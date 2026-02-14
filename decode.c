#include "base64.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int INV[256];

void base64_init(void) {
    for (int i = 0; i < 256; i++)
        INV[i] = -1;

    for (int i = 0; i < 64; i++)
        INV[(unsigned char)BASE64_ALPHABET[i]] = i;
}

void base64_decode(FILE *in, FILE *out) {
    char line[256];
    int lineno = 0;
    int finished = 0;

    while (fgets(line, sizeof(line), in)) {
        lineno++;

        size_t len = strcspn(line, "\r\n");
        line[len] = 0;

        if (line[0] == '-')
            continue;

        if (finished) {
            fprintf(stderr, "Line %d: data after end of message\n", lineno);
            exit(EXIT_FAILURE);
        }

        if (len % 4 != 0) {
            fprintf(stderr, "Line %d: invalid length\n", lineno);
            exit(EXIT_FAILURE);
        }

        for (size_t i = 0; i < len; i += 4) {
            int v[4], pad = 0;

            for (int j = 0; j < 4; j++) {
                char c = line[i+j];
                if (c == '=') {
                    v[j] = 0;
                    pad++;
                } else if (INV[(unsigned char)c] < 0) {
                    fprintf(stderr, "Line %d, char %zu: invalid character\n",
                            lineno, i+j+1);
                    exit(EXIT_FAILURE);
                } else {
                    v[j] = INV[(unsigned char)c];
                }
            }

            if (pad && i + 4 != len) {
                fprintf(stderr, "Line %d: padding in middle of data\n", lineno);
                exit(EXIT_FAILURE);
            }

            uint32_t x = (v[0]<<18) | (v[1]<<12) | (v[2]<<6) | v[3];

            fputc((x >> 16) & 0xFF, out);
            if (pad < 2) fputc((x >> 8) & 0xFF, out);
            if (pad < 1) fputc(x & 0xFF, out);

            if (pad)
                finished = 1;
        }
    }
}
