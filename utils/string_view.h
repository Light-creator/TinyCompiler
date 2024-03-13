#ifndef _STRING_VIEW_H
#define _STRING_VIEW_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "types.h"

typedef struct {
    u32 size;
    char *data;
} String_view;

#define SV(str) \
    (String_view) { \
        .size = (u32)strlen(str) - 1, \
        .data = str \
    }

// String_view create_sv(char *str);

void print_sv(String_view sv);

// String_view sv_from_char(char c);

char *cstr_from_ch(char c);

#endif