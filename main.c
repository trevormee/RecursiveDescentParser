/***************************************************************
  Student Name: Trevor Mee
  File Name: main.c
  Project 1

  @brief Contains the main function for the scanner. It initializes 
         the scanner, reads the input file, and prints the tokens 
         and lexemes.
***************************************************************/

#include "scanner.h"
#include "scanner.c"

#define MAX_FILE_SIZE 1024

int main(int argc, char *argv[]) {
    
    if(argc != 2) {
        fprintf(stderr, "Usage: %s <source file>\n", argv[0]);
        return 1;
    }

    // open the input file
    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    // convert the input file to a string
    char source[MAX_FILE_SIZE];
    size_t length = fread(source, 1, MAX_FILE_SIZE - 1, file);
    source[length] = '\0'; 
    fclose(file);

    // initialize scanner
    Scanner scanner; 
    initScanner(&scanner, source);

    // do the scanning
    Token token;
    do
    {
        token = nextToken(&scanner);
        printf("Token: %s", token.token);
        
         if (token.isNum) {
             printf(", Lexeme: %d", token.lexeme.num);
         } else if (token.lexeme.str != NULL) {
                 printf(", Lexeme: %s", token.lexeme.str);
        }
            printf("\n");
    }while(token.token != NULL && strcmp(token.token, eoIToken) != 0);

    free(scanner.source);
    
    return 0;
}
