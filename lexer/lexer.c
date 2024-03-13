#include <string.h>

#include "lexer.h"

Lexer create_lexer(char *source) {
    Lexer l;
    l.source = source;
    l.source_len = (i64)strlen(source);
    l.curr_char = ' ';
    l.curr_pos = -1;

    next_char(&l);
    return l;
}

char peek_char(Lexer *l) {
    if(l->curr_pos + 1 >= l->source_len) {
        return '\0';
    }

    return l->source[l->curr_pos+1];
}

void next_char(Lexer *l) {
    l->curr_pos++;

    if(l->curr_pos >= l->source_len) {
        l->curr_char = '\0';
    } else {
        l->curr_char = l->source[l->curr_pos];
    }
}

void skip_whitespace(Lexer *l) {
    while(l->curr_char == ' ' || l->curr_char == '\t' || l->curr_char == '\r') {
        next_char(l);
    }
}