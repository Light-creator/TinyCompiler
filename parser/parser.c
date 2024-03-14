#include "parser.h"

Parser create_parser(Lexer *l) {
    Parser p;
    p.lexer = l;

    p.curr_token = get_token(p.lexer);
    p.peek_token = get_token(p.lexer);

    p.tree = create_tree();

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
    switch(p->curr_token.tk_type) {
        case PRINT:
            tree_insert(&(p->tree), p->curr_token);
            next_token(p);

            if(p->curr_token.tk_type == STRING) {
                p->tree.curr_ptr->right = create_node(p->curr_token);
            } else {
                Node *expression_node = parse_expression(p);
                p->tree.curr_ptr->right = expression_node;  
            }
            
            next_token(p);
            break;
        case INPUT:
            tree_insert(&(p->tree), p->curr_token);
            next_token(p);
            
            p->tree.curr_ptr->right = create_node(p->curr_token);
            match(p, INDENT);
            break;
        default: 
            printf("Undefined token\n");
            exit(1);
    }

    new_line(p);
}

static int get_precedence(token_type tk_type) {
    // 2 - digit or indent
    // 0 - + or -
    // 1 - * or /
    switch(tk_type) {
        case NUMBER: case INDENT:
            return 2;
        case PLUS: case MINUS:
            return 0;
        case ASTRERISK: case SLASH:
            return 1;
        default:
            return -1;
    }

    return -1;
}

Node *parse_expression(Parser *p) {
    Token expr_buf[BUF_LEN];
    int size_buf = 0;
    
    while(size_buf < BUF_LEN) {
        expr_buf[size_buf] = p->curr_token;
        size_buf++;
        if(p->peek_token.tk_type == NEWLINE) {
            break;
        }
        next_token(p);
    }

    int min_precedence = 3;
    int min_idx = -1;
    for(int i=0; i<size_buf; i++) {
        int precedence = get_precedence(expr_buf[i].tk_type);
        if(min_precedence >= precedence) {
            min_precedence = precedence;
            min_idx = i;
        }
    }

    assert(min_idx != -1);
    
    Node *node = create_node(expr_buf[min_idx]);

    node->right = parse_expression_part(expr_buf, min_idx+1, size_buf);
    node->left = parse_expression_part(expr_buf, 0, min_idx);

    return node;
}

Node *parse_expression_part(Token *expr_buf, int start, int end) {
    // printf("%d %d ", start, end);
    if(end - start <= 1) {
        // printf("\n %s\n", expr_buf[start].tk_text.data);
        Node *new_node = create_node(expr_buf[start]);
        printf("%s\n", new_node->value.tk_text.data);
        return new_node;
    }

    int min_precedence = 3;
    int min_idx = -1;
    for(int i=start; i<end; i++) {
        int precedence = get_precedence(expr_buf[i].tk_type);
        if(min_precedence >= precedence) {
            min_precedence = precedence;
            min_idx = i;
        }
    } 

    assert(min_idx != -1);
    
    Node *node = create_node(expr_buf[min_idx]);

    // printf("%d\n", min_idx);
    node->right = parse_expression_part(expr_buf, min_idx+1, end);
    node->left = parse_expression_part(expr_buf, start, min_idx);

    return node;
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

void new_line(Parser *p) {
    p->tree.curr_ptr->left = create_node(p->curr_token);
    p->tree.curr_ptr = p->tree.curr_ptr->left;
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