#ifndef _PARSER_H
#define _PARSER_H

#include <stdbool.h>

#include "../lexer/lexer.h"
#include "../token/token.h"

#define BUF_LEN 256
#define DATA_LEN 2048
#define TEXT_LEN 4096

typedef struct _var {
    String_view title;
    Token value;
} Var;

typedef struct _parser {
    Lexer *lexer;

    Token curr_token;
    Token peek_token;

    Tree ast;
    // HashMap vars;

    // Var vars[BUF_LEN];
    // i32 vars_size;

    char *data_section;
    char *text_section;

    char *data_offset;
    char *text_offset;

    int count_data_items;
} Parser;

Parser create_parser(Lexer *l);

void program(Parser *p);
void statement(Parser *p);

bool check_token(Parser *p, token_type tk_type);
bool check_peek_token(Parser *p, token_type tk_type);
bool is_compare_signs(Parser *p);

void next_token(Parser *p);
void match(Parser *p, token_type tk_type);
void new_line();

void expression(Parser *p);
void comparison(Parser *p);
void term(Parser *p);
void unary(Parser *p);
void primary(Parser *p);

#endif