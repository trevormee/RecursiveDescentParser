/***************************************************************
  Student Name: Trevor Mee
  File Name: scanner.h
  Project 1

  @brief This file defines the structures and functions for a 
         scanner. It includes token structure, keyword and operator
         mapping, and function declarations for the scanner.
***************************************************************/

#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define MAX_IDENTIFIER_LENGTH 50

const char EOI = EOF; 
const char START_COMMENT = '~';
const char START_STRING = '"';
const char END_STRING = '"';
const char EQUAL = '=';
const char NOT = '!';
const char GREATER = '>';
const char LESS = '<';
#define WHITESPACE " \t\n\r"
const char *DIGITS = "0123456789";
const char *LETTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
const char *LETTERS_DIGITS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_";


// mapping for operator table
typedef struct {
    char op;
    const char *token;
} OpMapping;

// operator table
OpMapping OP_TABLE[] = {
    { '(', "lParen" },
    { ')', "rParen" },
    { '{', "lCurly" },
    { '}', "rCurly" },
    { '+', "plusSym" },
    { '-', "minusSym" },
    { '*', "timesSym" },
    { '/', "divSym" },
    { ';', "semicolon" },
    { ',', "comma" }
};

const int OP_TABLE_SIZE = sizeof(OP_TABLE) / sizeof(OP_TABLE[0]);

// mapping for keyword table
typedef struct{
    const char *keyword;
    const char *token;
}KeywordMapping;

// keyword table
KeywordMapping KEYWORD_TABLE[] = {
    { "while",  "whileSym" },
    { "return", "returnSym" },
    { "if",     "ifSym" },
    { "else",   "elseSym" },
    { "do",     "doSym" },
    { "int",    "intSym" },
    { "string", "stringSym" },
    { "begin",  "beginSym" },
    { "end",    "endSym" }
};
const int KEYWORD_TABLE_SIZE = sizeof(KEYWORD_TABLE) / sizeof(KEYWORD_TABLE[0]);

const char *eoIToken = "end.";

// token structure
typedef struct {
    char *token;   // Token class name
    int isNum;     // Flag: 1 if lexeme is a number, 0 otherwise
    union {
        int num;
        char *str;
    } lexeme;
} Token;

// scanner state structure
typedef struct {
    char *source;      // The source string (with appended EOI)
    int position;      // Current position in source
    char *currentText; // Current text/token (not used actively)
    char *currentToken;// Current token (not used actively)
} Scanner;

// function declarations
void initScanner(Scanner *self, const char *source);
int inSet(char ch, const char *set);
void error(Scanner *self, const char *message);
char currentCh(Scanner *self);
void move(Scanner *self);
int atEOI(Scanner *self);
void eat(Scanner *self);
char *find(Scanner *self, char x);
char *findStar(Scanner *self, const char *s);
char *skip(Scanner *self, char *x);
char *skipStar(Scanner *self, const char *s);
void skipWhitespace(Scanner *self);
void skipComment(Scanner *self);
void jump(Scanner *self);
void jumpStar(Scanner *self);
Token NUM(Scanner *self);
Token ID(Scanner *self);
Token STRI(Scanner *self);
char *twoCharSym(Scanner *self, char secondCh, const char *firstToken, const char *secondToken);
Token nextToken(Scanner *self);

#endif 