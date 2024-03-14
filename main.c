#include <stdio.h>
#include <string.h>

#include "utils/types.h"
#include "utils/string_view.h"
#include "lexer/lexer.h"
#include "token/token.h"
#include "parser/parser.h"

static char *read_content(const char *filename);

Lexer lexer;
Parser parser;

int main() {
    // char *input = "let a = \"hello\" ";
    // char *input = "+- !=<=>= != ! -*";
    char *input = read_content("prog_examples/example.tl");
    FILE *file = fopen("main.asm", "w");
    // lexer = create_lexer(input);

    // Token token = get_token(&lexer);
    // while(token.tk_type != _EOF) {
    //     print_sv(token.tk_text);
    //     token = get_token(&lexer);
    // }

    lexer = create_lexer(input);
    parser = create_parser(&lexer);

    // while(parser.curr_token.tk_type != _EOF) {
    //     printf("- %s\n", parser.curr_token.tk_text.data);
    //     next_token(&parser);
    // }

    program(&parser);
    print_tree(&(parser.tree));
    fclose(file);

    return 0;
}

static char *read_content(const char *filename) {
    char *fcontent = NULL;
    int fsize = 0;
    FILE *fp;

    fp = fopen(filename, "r");
    if(fp) {
        fseek(fp, 0, SEEK_END);
        fsize = ftell(fp);
        rewind(fp);

        fcontent = (char*) malloc(sizeof(char) * fsize);
        fread(fcontent, 1, fsize, fp);

        fclose(fp);
    }
    return fcontent;
}