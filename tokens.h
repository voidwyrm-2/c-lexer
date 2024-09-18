#pragma once

typedef enum Tokentype {
    TT_PLUS,
    TT_HYPHEN,
    TT_ASTERISK, 
    TT_FORWARD_SLASH,
    TT_NUMBER
} Tokentypes;

typedef struct Token {
    enum Tokentype type;
    char* lit;
    int start, end, ln;
} Token;

Token *NewToken(enum Tokentype type, char* lit, int start, int end, int ln) {
    Token *t = malloc(sizeof(Token));
    t->type = type;
    t->lit = lit;
    t->start = start;
    t->end = end;
    t->ln = ln;
    return t;
}

void PrintToken(Token *t) {
    printf("Token{%d, \"%s\", %d..%d, %d}\n", t->type, t->lit, t->start, t->end, t->ln);
}
