
#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_IDENTIFIER_LENGTH 50

// Token Types
typedef enum {
//  Token Name             Description           Lexeme examples
//  -------------------------------------------------------------
    TOKEN_IDENTIFIER,   // variable names        "x", "a1"
    TOKEN_NUMBER,       // numeric literals      "1", "999"
    TOKEN_ASSIGN,       // assignment operator   "="
    TOKEN_SEMICOLON,    // statement terminator   ";"
    TOKEN_BEGIN,        // begin reserved word   "BEGIN"
    TOKEN_END,          // end reserved word     "END"
    TOKEN_LPAREN,       // left parenthesis      "("
    TOKEN_RPAREN,       // right parenthesis     ")"
    TOKEN_LCURLY,       // left curly brace      "{"
    TOKEN_RCURLY,       // right curly brace     "}"
    TOKEN_OPERATOR,     // arithmetic operators   "+", "-", "*", "/"
    TOKEN_COMMA,        // comma                 ","
    TOKEN_COMMENT,      // comment               "~ some comment"
    TOKEN_EOF,          // end of file           "EOF"
    TOKEN_ERROR,        // error token           "ERROR"
    TOKEN_WHITESPACE,   // whitespace            " ", "\t", "\n"
} TokenType;

// Token Structure
typedef struct {
    TokenType type;      
    char lexeme[MAX_IDENTIFIER_LENGTH]; 
    int line;        
} Token;

// Function Declarations
void initScanner(const char *fileName);   // initialize the scanner with the source file
Token getNextToken();   // get the next token from the source file
void error(const char *message); // print error message 
void currentCharacter(); // get current character
void move();        // advance 1 char
bool eoi();        // end of file
void eat();        // same as move, but report error of moving past eoi
// def find()
// def findStar()
// def skip
// def skipStar()
void skipWhitespace();  // skip whitespace
void skipComment();     // skip comments
void jump();            // move over whitespace or comment
void jumpStar();


void closeScanner();


#endif // SCANNER_H
