#include <stdlib.h>
#include <stdio.h>
#include "lexer.h"
#include "tokens.h"

int main() {
    Lexer *lexer = NewLexer("1 1 + 2 *");
    
    LexerResult res = Lex(lexer);
    if (res.err != NULL) {
        printf("%s\n", res.err);
        return 1;
    }

    for (int i = 0; i < res.tokens.len; i++) {
        PrintToken((Token*)res.tokens.arr[i]);
    }

    return 0;
}
