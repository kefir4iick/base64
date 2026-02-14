#include "base64.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define MAX_COMMENT_LEN 76
#define LINE_LEN 76

const char BASE64_ALPHABET[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"; // сначала не пон как \ написать. чекнула, можно \\, но по стандарту /. так шо будет /

void write_comment(FILE *out, const char *comment) {
    if (!comment || !*comment)
        return;

    size_t len = strlen(comment);
    size_t pos = 0;

    while (pos < len) {
        size_t chunk = len - pos;
        if (chunk > MAX_COMMENT_LEN)
            chunk = MAX_COMMENT_LEN;

        fputc('-', out);
        fputc(' ', out);
        fwrite(comment + pos, 1, chunk, out);
        fputc('\n', out);

        pos += chunk;
    }
}

void base64_encode(FILE *in, FILE *out, const char *comment) {
    unsigned char buf[3];
    int n, col = 0;

    write_comment(out, comment);

    while ((n = fread(buf, 1, 3, in)) > 0) {
        uint32_t x = 0;
        for (int i = 0; i < n; i++)
            x |= (uint32_t)buf[i] << (16 - 8 * i);

        char o[4];
        o[0] = BASE64_ALPHABET[(x >> 18) & 0x3F];
        o[1] = BASE64_ALPHABET[(x >> 12) & 0x3F];
        o[2] = (n >= 2) ? BASE64_ALPHABET[(x >> 6) & 0x3F] : '=';
        o[3] = (n == 3) ? BASE64_ALPHABET[x & 0x3F] : '=';

        for (int i = 0; i < 4; i++) {
            if (col == LINE_LEN) {
                fputc('\n', out);
                col = 0;
            }
            fputc(o[i], out);
            col++;
        }
    }

    if (col > 0)
        fputc('\n', out);
}

#ifdef BASE64_MAIN
int main(void) {
    const char *text = "hello";
    FILE *in = fmemopen((void *)text, strlen(text), "r"); //для линукса и макоса норм бо fmemopen входит в стандарт посикс. но для винды по идее не буде працювати
    
    if (!in) {
        perror("fmemopen failed");
        return 1;
    }
    
    base64_encode(in, stdout, "Example comment");
    fclose(in);
    
    return 0;
}
#endif
