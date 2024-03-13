#ifndef _LEXER_H
#define _LEXER_H

#include "../utils/types.h"

typedef struct {
    char *source;
    char curr_char;
    i32 curr_pos;
    i64 source_len; 
} Lexer;

Lexer create_lexer(char *source);
void next_char(Lexer *l);
char peek_char(Lexer *l);
void skip_whitespace(Lexer *l);

#endif