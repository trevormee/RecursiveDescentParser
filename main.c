/***************************************************************
  Student Name: Trevor Mee
  File Name: main.c
  Project 1

  @brief 
***************************************************************/

#include "scanner.h"
#include "scanner.c"

#define MAX_FILE_SIZE 1024

int main(int argc, char *argv[]) {
    
    if(argc != 2) {
        fprintf(stderr, "Usage: %s <source file>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    char source[MAX_FILE_SIZE];
    size_t length = fread(source, 1, MAX_FILE_SIZE - 1, file);
    source[length] = '\0'; // Null-terminate the string
    fclose(file);

    //const char *source = "a=10; b = 5;  $";
    Scanner scanner; 
    initScanner(&scanner, source);
    //initScanner(&scanner,argv[1]);

    Token token;
    do
    {
        token = nextToken(&scanner);
        printf("Token: %s", token.token);
        
         if (token.isNum) {
             printf(", Lexeme (num): %d", token.lexeme.num);
         } else if (token.lexeme.str != NULL) {
                 printf(", Lexeme: %s", token.lexeme.str);
        }
            printf("\n");
            //free(token.token);
        //     if (token.lexeme.str != NULL) {
        //         free(token.lexeme.str);
        //     }
        // } else {
        //     break;
    }while(token.token != NULL && strcmp(token.token, eoIToken) != 0);

    free(scanner.source);
    
    return 0;
}
