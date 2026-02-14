#ifndef BASE64_H
#define BASE64_H

#include <stdio.h>

#define LINE_LEN 76

extern const char BASE64_ALPHABET[64];
extern int INV[256];

void base64_init(void);

void base64_encode(FILE *in, FILE *out, const char *comment);

void base64_decode(FILE *in, FILE *out);

#endif
