#include "token.h"
#include <stdbool.h>


bool is_digit(char c) {
    if(c >= '0' && c <= '9')
        return true;

    return false;
}

bool is_alphabetic(char c) {
    if(c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '"' || c == '_' || c == '\'')
        return true;

    return false;
}

bool is_correct_str(char c) {
    if(c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '"' || c == '_' || c == '\'' || c == ' ' || c == '!')
        return true;
    else if(is_digit(c))
        return true;

    return false;
}

Token read_string(Lexer *l) {
    next_char(l);
    int size_s = 0;
    int start_pos = l->curr_pos;

    while(l->curr_char != ' ' && l->curr_char != '\0' && l->curr_char != '"' && l->curr_char != '\'' && l->curr_char != '\n') {
        //printf("%c %d\n", l->curr_char, is_alphabetic(l->curr_char));
        if(is_correct_str(l->curr_char)) {
            size_s++;
        } else {
            return TINY_TK("ERR", ERROR);
        }

        if(peek_char(l) == '\n')
            break;

        next_char(l);
    }

    char *s = (char*)malloc(sizeof(char)*size_s);
    strncpy(s, l->source+start_pos, size_s);
    s[size_s] = '\0';

    return TINY_TK(s, STRING);
}

Token read_alphabetic(Lexer *l) {
    int size_s = 0;
    int start_pos = l->curr_pos;
    bool string_flag = false;

    if(l->curr_char == '"' || l->curr_char == '\'') {
        return read_string(l);
    }

    while(l->curr_char != ' ' && l->curr_char != '\0' && l->curr_char != '"' && l->curr_char != '\'' && l->curr_char != '\n') {
        //printf("%c %d\n", l->curr_char, is_alphabetic(l->curr_char));
        if(is_alphabetic(l->curr_char)) {
            size_s++;
        } else if(is_digit(l->curr_char) && size_s != 0) {
            size_s++;
        } else {
            return TINY_TK("ERR", ERROR);
        }

        if(peek_char(l) == '\n')
            break;

        next_char(l);
    }

    char *s = (char*)malloc(sizeof(char)*size_s);
    strncpy(s, l->source+start_pos, size_s);
    s[size_s] = '\0';
    
    if(strncmp(s, "let", 3) == 0) {
        return TINY_TK(s, LET);
    } else if(strncmp(s, "goto", 4) == 0) {
        return TINY_TK(s, GOTO);
    } else if(strncmp(s, "print", 5) == 0) {
        return TINY_TK(s, PRINT);
    } else if(strncmp(s, "label", 5) == 0) {
        return TINY_TK(s, LABEL);
    } else if(strncmp(s, "input", 5) == 0) {
        return TINY_TK(s, INPUT);
    } else if(strncmp(s, "if", 2) == 0) {
        return TINY_TK(s, IF);
    } else if(strncmp(s, "endif", 5) == 0) {
        return TINY_TK(s, ENDIF);
    } else if(strncmp(s, "while", 5) == 0) {
        return TINY_TK(s, WHILE);
    } else if(strncmp(s, "endwhile", 8) == 0) {
        return TINY_TK(s, ENDWHILE);
    } else if(strncmp(s, "repeat", 6) == 0) {
        return TINY_TK(s, REPEAT);
    }

    return TINY_TK(s, INDENT);
}

Token read_numeric(Lexer *l) {
    int size_s = 0;
    int start_pos = l->curr_pos;

    while(l->curr_char != ' ' && l->curr_char != '\0') {
        if(is_digit(l->curr_char)) {
            size_s++;
        } else {
            break;
        }

        if(!is_digit(peek_char(l)) || peek_char(l) == '\n')
            break;

        next_char(l);
    }

    char *s = (char*)malloc(sizeof(char)*size_s);
    strncpy(s, l->source+start_pos, size_s);
    s[size_s] = '\0';

    return TINY_TK(s, NUMBER);
}

Token get_token(Lexer *l) {
    Token token;
    skip_whitespace(l);

    switch(l->curr_char) {
        case '+':
            token = TINY_TK(cstr_from_ch('+'), PLUS);
            break;
        case '-':
            token = TINY_TK(cstr_from_ch('-'), MINUS);
            break;
        case '/':
            token = TINY_TK(cstr_from_ch('/'), SLASH);
            break;
        case '*':
            token = TINY_TK(cstr_from_ch('*'), ASTRERISK);
            break;
        case '=':
            if(peek_char(l) == '=') {
                token = TINY_TK("==", EQEQ);
                next_char(l);
            } else {
                token = TINY_TK(cstr_from_ch('='), EQ);
            }
            break;
        case '!':
            if(peek_char(l) == '=') {
                token = TINY_TK("!=", NOTEQ);
                next_char(l);
            } else {
                token = TINY_TK(cstr_from_ch('!'), BANG);
            }
            break;
        case '>':
            if(peek_char(l) == '=') {
                token = TINY_TK(">=", GTEQ);
                next_char(l);
            } else {
                token = TINY_TK(cstr_from_ch('>'), GT);
            }
            break;
        case '<':
            if(peek_char(l) == '=') {
                token = TINY_TK("<=", LTEQ);
                next_char(l);
            } else {
                token = TINY_TK(cstr_from_ch('<'), LT);
            }
            break;
        case '\0':
            token = TINY_TK(cstr_from_ch('\0'), _EOF);
            break;
        case '\n':
            token = TINY_TK("\n", NEWLINE);
            break;
        default:
            if(is_alphabetic(l->curr_char)) {
                token = read_alphabetic(l);
            } else if(is_digit(l->curr_char)) {
                token = read_numeric(l);
            } else {
                token = TINY_TK("\0", UNDEFINED);
                printf("Undefined token %c\n", l->curr_char);
            }
            break;
    }
    
    next_char(l);
    return token;
}

void print_token_type(token_type tk_type) {
    switch(tk_type) {
        case _EOF: printf("_EOF"); break;
        case UNDEFINED: printf("UNDEFINED"); break;
        case ERROR: printf("ERROR"); break;
        case NEWLINE: printf("NEWLINE"); break;
        case INDENT: printf("INDENT"); break;
        case STRING: printf("STRING"); break;
        case NUMBER: printf("NUMBER"); break;

        case LABEL: printf("LABEL"); break;
        case GOTO: printf("GOTO"); break;
        case PRINT: printf("PRINT"); break;
        case INPUT: printf("INPUT"); break;
        case LET: printf("LET"); break;
        case ENDIF: printf("ENDIF"); break;
        case WHILE: printf("WHILE"); break;
        case REPEAT: printf("REPEAT"); break;
        case THEN: printf("THEN"); break;
        case ENDWHILE: printf("ENDWHILE"); break;

        case EQ: printf("EQ"); break;
        case PLUS: printf("PLUS"); break;
        case MINUS: printf("MINUS"); break;
        case ASTRERISK: printf("ASTRERISK"); break;
        case SLASH: printf("SLASH"); break;
        case NOTEQ: printf("NOTEQ"); break;
        case LT: printf("LT"); break;
        case LTEQ: printf("LTEQ"); break;
        case GT: printf("GT"); break;
        case GTEQ: printf("GTEQ"); break;
        case BANG: printf("BANG"); break;
        default: break;
    }
}