#include "parser.h"

Parser create_parser(Lexer *l) {
    Parser p;
    p.lexer = l;

    p.curr_token = get_token(p.lexer);
    p.peek_token = get_token(p.lexer);

    p.vars_size = 0;

    p.data_section = (char*)malloc(sizeof(char)*DATA_LEN);
    p.text_section = (char*)malloc(sizeof(char)*TEXT_LEN);

    strncpy(p.data_section, "section .data\n", 14);
    strncpy(p.text_section, "section .text\nglobal _start\n_start:\n", 36);

    p.data_offset = p.data_section + 14;
    p.text_offset = p.text_section + 36;

    p.count_data_items = 0;

    return p;
}

void program(Parser *p) {
    printf("Start Program:\n");

    while(!check_token(p, _EOF)) {
        statement(p);
    }

    // return 0
    // strncpy(p->text_offset, "\nmov rax,60\nsyscall", 49);
    // p->text_offset += 49;
}

void statement(Parser *p) {

}

bool check_token(Parser *p, token_type tk_type) {
    if(p->curr_token.tk_type == tk_type)
        return true;

    return false;
}

bool is_compare_signs(Parser *p) {
    bool flag_cmp = false;
    token_type tk_type_arr[6] = {EQEQ, NOTEQ, LT, LTEQ, GT, GTEQ};
    for(int i=0; i<6; i++) {
        if(p->curr_token.tk_type == tk_type_arr[i])
            flag_cmp = true;
    }

    return flag_cmp;
}

void next_token(Parser *p) {
    p->curr_token = p->peek_token;
    p->peek_token = get_token(p->lexer);
}

void match(Parser *p, token_type tk_type) {
    if(p->curr_token.tk_type != tk_type) {
        printf("Parsing Error: %s | Expect ", p->curr_token.tk_text.data);
        print_token_type(tk_type);
        printf("\n");
        exit(1);
    }
    next_token(p);
}

void comparison(Parser *p) {
    printf("Comprasion\n");
    
    expression(p);

    if(is_compare_signs(p)) {
        next_token(p);
        expression(p);
    } else {
        printf("Parsing Error in Comparison: Expected comapre sign\n");
        exit(1);
    }

    while(is_compare_signs(p)) {
        next_token(p);
        expression(p);
    }
}

void expression(Parser *p) {
    printf("Expression\n");
    
    term(p);

    while(check_token(p, PLUS) || check_token(p, MINUS)) {
        next_token(p);
        term(p);
    }
}

void term(Parser *p) {
    printf("Term\n");

    unary(p);

    while(check_token(p, ASTRERISK) || check_token(p, SLASH)) {
        next_token(p);
        unary(p);
    }
}

void unary(Parser *p) {
    printf("Unary\n");

    while(check_token(p, PLUS) || check_token(p, MINUS)) {
        next_token(p);
    }

    primary(p);
}

void primary(Parser *p) {
    printf("Primary (%s)\n", p->curr_token.tk_text.data);

    if(check_token(p, NUMBER)) {
        next_token(p);
    } else if(check_token(p, INDENT)){
        next_token(p);
    } else {
        printf("Parsing Error in Primary: Expected NUMBER or IDENT\n");
        exit(1);
    }
}

void new_line(Parser *p) {
    printf("New Line\n");

    match(p, NEWLINE);

    while(check_token(p, NEWLINE))
        next_token(p);
}


// void statement(Parser *p) {

//     // print
//     if(p->curr_token.tk_type == PRINT) {
//         next_token(p);

//         if(check_token(p, STRING)) {
//             char str_tmp[100];
//             snprintf(str_tmp, 100, "str%d db '%s', 0xa\n", p->count_data_items, p->curr_token.tk_text.data);
//             p->count_data_items++;

//             strncpy(p->data_offset, str_tmp, strlen(str_tmp));
//             p->data_offset += strlen(str_tmp);

//             char str_text_sec[200];
//             snprintf(str_text_sec, 200, "mov rax,1\nmov rdi,1\nmov rdx,%d\nmov rsi, str%d\nsyscall", 6, i);
//             strncpy(p->text_offset, str_text_sec, strlen(str_text_sec));
//             p->text_offset += strlen(str_text_sec);

//             next_token(p);
//         } else {
            
//             expression(p);
//         }
//     } else if(p->curr_token.tk_type == LET) {
//         printf("Let-statement\n");
//         next_token(p);

//         Var v;
//         v.title = p->peek_token.tk_text;

//         match(p, INDENT);
//         match(p, EQ);

//         expression(p);
//     } else if(p->curr_token.tk_type == IF) {
//         printf("If-statement\n");
//         next_token(p);

//         comparison(p);

//         match(p, THEN);
//         new_line(p);

//         while(!check_token(p, ENDIF))
//             statement(p);
//     } else if(p->curr_token.tk_type == WHILE) {
//         printf("While-statement\n");
//         next_token(p);

//         comparison(p);

//         match(p, REPEAT);
//         new_line(p);

//         while(!check_token(p, ENDWHILE))
//             statement(p);

//         match(p, ENDWHILE);
//     } else if(p->curr_token.tk_type == GOTO) {
//         printf("Goto-statement\n");
//         next_token(p);

//         match(p, INDENT);
//     } else if(p->curr_token.tk_type == LABEL) {

//     } else if(p->curr_token.tk_type == INPUT) {
//         printf("Input-statement\n");
//         next_token(p);

//         match(p, INDENT);
//     } else if(p->curr_token.tk_type == INDENT) {
//         printf("Var-statement\n");
//         next_token(p);

//         match(p, EQ);
//         expression(p);

//         // p->vars[p->vars_size] = p->curr_token.tk_text;
//         // p->vars_size++;
//     } else {
//         printf("Parsing Error: Undefined statement (%s)\n", p->curr_token.tk_text.data);
//         exit(1);
//     }

//     new_line(p);
// }