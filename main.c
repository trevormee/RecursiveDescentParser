// #include <stdio.h>
// #include "scanner.h"

// int main(int argc, char *argv[]) {
//     if (argc != 2) {
//         fprintf(stderr, "Usage: %s <source file>\n", argv[0]);
//         return 1;
//     }

//     initScanner(argv[1]);

//     Token token;
//     do {
//         token = getNextToken();
//         printf("Token: %-15s Lexeme: %-10s Line: %d\n", 
//             (token.type == TOKEN_IDENTIFIER) ? "IDENTIFIER" :
//             (token.type == TOKEN_NUMBER) ? "NUMBER" :
//             (token.type == TOKEN_ASSIGN) ? "ASSIGN" :
//             (token.type == TOKEN_SEMICOLON) ? "SEMICOLON" :
//             (token.type == TOKEN_BEGIN) ? "BEGIN" :
//             (token.type == TOKEN_END) ? "END" :
//             (token.type == TOKEN_LPAREN) ? "LPAREN" :
//             (token.type == TOKEN_RPAREN) ? "RPAREN" :
//             (token.type == TOKEN_OPERATOR) ? "OPERATOR" :
//             (token.type == TOKEN_COMMENT) ? "COMMENT" :
//             (token.type == TOKEN_EOF) ? "EOF" : "ERROR",
//             token.lexeme, token.line);
//     } while (token.type != TOKEN_EOF);

//     closeScanner();
//     return 0;
// }

#include "scanner.h"
#include "scanner.c"

int main(int argc, char *argv[])
{
    
    if(argc != 2) {
        fprintf(stderr, "Usage: %s <source file>\n", argv[0]);
        return 1;
    }
    initScanner(argv[1]);

    return 0;
}