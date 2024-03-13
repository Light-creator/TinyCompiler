#include "string_view.h"

// String_view create_sv(char *str) {
//     String_view sv;
//     sv.size = strlen(str) - 1;
//     sv.data = str;

//     return sv;
// }

void print_sv(String_view sv) {
    printf("%s\n", sv.data);
}

// String_view sv_from_char(char c) {
//     char *s = (char*)malloc(2 * sizeof(char));
//     s[0] = c; s[1] = '\0';
//     return SV(s);
// }

char *cstr_from_ch(char c) {
    char *s = (char*)malloc(2 * sizeof(char));
    s[0] = c; s[1] = '\0';
    return s;
}