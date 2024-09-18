#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/nucl.h"
#include "tokens.h"

typedef struct LexerResult {
    Arraylist tokens;
    char* err;
} LexerResult;

typedef struct Lexer {
    char* text;
    int idx, ln;
    char cchar;
} Lexer;

void advance(Lexer* l) {
    l->idx++;
    l->cchar = l->idx < strlen(l->text) ? l->text[l->idx] : 0;
    if (l->cchar == '\n') l->ln++;
}

Lexer* NewLexer(char* text) {
    Lexer* l = (Lexer*)malloc(sizeof(Lexer));
    l->text = text;
    l->idx = -1;
    l->ln = 0;
    l->cchar = 0;
    advance(l);
    return l;
}

size_t GetLengthOfNumber(int n) {
    int l = 1;
    int i = 10;
    while (1) {
        if (n - i < 0) break;
        i *= 10;
        l += 1;
    }
    return l;
}

char* NewIllegalCharacterError(int ln, char ch) {
    char* out = (char*)malloc(sizeof("error on line : illegal character ''") +
                              1 + GetLengthOfNumber(ln));
    sprintf(out, "error on line %d: illegal character '%c'", ln, ch);
    return out;
}

Token* CollectNumber(Lexer* l) {
    int start = l->idx;
    int le = 0;
    char* num_str = NULL;

    while (l->cchar != 0 && l->cchar >= '0' && l->cchar <= '9') {
        le++;
        char* ns = malloc(le);
        if (le > 0) {
            for (int i = 0; i < le - 1; i++) {
                ns[i] = num_str[i];
            }
            if (num_str != NULL)
                free(num_str);
        }
        ns[le - 1] = l->cchar;        num_str = ns;
        advance(l);
    }

    return NewToken(TT_NUMBER, num_str, start, l->idx - 1,
                    l->cchar == '\n' ? l->ln - 1 : l->ln);
}

LexerResult Lex(Lexer* l) {
    Arraylist tokens = NewArraylist(1);
    LexerResult result = {.tokens = tokens, .err = NULL};

    while (l->cchar != 0) {
        switch (l->cchar) {
            case '\n':
            case '\t':
            case ' ':
                advance(l);
                break;

            case '+': {
                char* s = (char*)malloc(1);
                s[0] = l->cchar;
                ALAdd(&tokens,
                      (void*)NewToken(TT_PLUS, s, l->idx, l->idx, l->ln));
                advance(l);
            } break;

            case '-': {
                char* s = (char*)malloc(1);
                s[0] = l->cchar;
                ALAdd(&tokens,
                      (void*)NewToken(TT_HYPHEN, s, l->idx, l->idx, l->ln));
                advance(l);
            } break;

            case '*': {
                char* s = (char*)malloc(1);
                s[0] = l->cchar;
                ALAdd(&tokens,
                      (void*)NewToken(TT_ASTERISK, s, l->idx, l->idx, l->ln));
                advance(l);
            } break;

            case '/': {
                char* s = (char*)malloc(1);
                s[0] = l->cchar;
                ALAdd(&tokens, (void*)NewToken(TT_FORWARD_SLASH, s, l->idx,
                                               l->idx, l->ln));
                advance(l);
            } break;

            default:
                if (l->cchar >= '0' && l->cchar <= '9') {
                    ALAdd(&tokens, (void*)CollectNumber(l));
                } else {
                    result.err = NewIllegalCharacterError(l->ln, l->cchar);
                    return result;
                }
        }
    }

    result.tokens = tokens;
    return result;
}
