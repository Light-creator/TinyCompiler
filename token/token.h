#ifndef _TOKEN_H
#define _TOKEN_H

#include "../utils/types.h"
#include "../utils/string_view.h"
#include "../lexer/lexer.h"

#define LEN_TEXT 256

typedef enum {
    _EOF,
    UNDEFINED,
    ERROR,
    NEWLINE,
    INDENT,
    STRING,
    NUMBER,
    
    LABEL,
    GOTO,
    PRINT,
    INPUT,
    LET,
    IF,
    ENDIF,
    WHILE,
    REPEAT,
    THEN,
    ENDWHILE,

    EQ,
    PLUS,
    MINUS,
    ASTRERISK,
    SLASH,
    BRL, // (
    BRR, // )
    EQEQ,
    NOTEQ,
    LT,
    LTEQ,
    GT,
    GTEQ,
    COLON,
    BANG,
} token_type;

typedef struct {
    token_type tk_type;
    String_view tk_text;
} Token;

#define TINY_TK(str, type) \
    (Token) { \
        .tk_text = SV(str), \
        .tk_type = type \
    }

Token get_token(Lexer *l);

void print_token_type(token_type tk_type);

#endif