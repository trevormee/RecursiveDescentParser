/***************************************************************
  Student Name: Trevor Mee
  File Name: scanner.c
  Project 1

  @brief This file defines the function definitions for the 
         scanner. It includes three levels of functions: lowest level
         functions, second level movements, and high level tokenizers.
***************************************************************/

#include "scanner.h"

//------------------------------
// A. Lowest level functions
//------------------------------

/*
    @brief
    @param
    @return
*/
void initScanner(Scanner *self, const char *source)
{
    int len = strlen(source);
    self->source = (char *)malloc(len + 2);
    strcpy(self->source, source);
    self->source[len] = EOI;
    self->source[len + 1] = '\0';

    self->position = 0;
               
    self->currentText = NULL;     
    self->currentToken = NULL; 
}


/*
    @brief
    @param
    @return
*/
int inSet(char ch, const char *set)
{
    while (*set) {
        if (ch == *set) return 1;
        set++;
    }
    return 0;
}


/*
    @brief
    @param
    @return
*/
void error(Scanner *self, const char *message)
{
    printf(">>> Error: %s\n", message);
}


/*
    @brief
    @param
    @return
*/
char currentCh(Scanner *self)
{
    return self->source[self->position];
}


/*
    @brief
    @param
    @return
*/
void move(Scanner *self)
{
    if (self->position + 1 < strlen(self->source)) {
        self->position += 1;
    } else {
        error(self, "Attempted to move past eoi!");
    }
}

/*
    @brief
    @param
    @return
*/
int atEOI(Scanner *self)
{
    return currentCh(self) == EOI;
}

//------------------------------
// B. Second level movements
//------------------------------

/*
    @brief
    @param
    @return
*/
void eat(Scanner *self)
{
    if (atEOI(self)) {
        error(self, "Cannot move beyond eoi!");
    } else {
        move(self);
    }
}


/*
    @brief
    @param
    @return
*/
char *find(Scanner *self, char x)
{
    char result[256] = {0};
    int index = 0;
    while (currentCh(self) != x && !atEOI(self)) {
        result[index++] = currentCh(self);
        result[index] = '\0';
        move(self);
    }
    if (atEOI(self)) {
        char errMsg[256];
        sprintf(errMsg, "eoi detected searching for %c", x);
        error(self, errMsg);
        return strdup(result);
    } else {
        return strdup(result);
    }
}


/*
    @brief
    @param
    @return
*/
char *findStar(Scanner *self, const char *s)
{
    char buffer[256] = {0};
    int idx = 0;
    while (!inSet(currentCh(self), s) && !atEOI(self)) {
        buffer[idx++] = currentCh(self);
        buffer[idx] = '\0';
        //move(self);
        eat(self);
    }
    if (atEOI(self)) {
        char errMsg[256];
        sprintf(errMsg, "eoi detected searching for %s", s);
        error(self, errMsg);
        return strdup(buffer);
    } else {
        return strdup(buffer);
    }
}
/*
    @brief
    @param
    @return
*/
char *skip(Scanner *self, char *x)
{
    char result[256] = {0};
    int index = 0;
    while (currentCh(self) == x[0]) {
        result[index++] = currentCh(self);
        result[index] = '\0';
        eat(self);
    }

    return strdup(result);
}
/*
    @brief
    @param
    @return
*/
char *skipStar(Scanner *self, const char *s)
{
    char result[256] = {0};
    int index = 0;
    while (inSet(currentCh(self), s)) {
        result[index++] = currentCh(self);
        result[index] = '\0';
        eat(self);
    }

    return strdup(result);
}
/*
    @brief
    @param
    @return
*/
void skipWhitespace(Scanner *self)
{
    char *temp = skipStar(self, WHITESPACE);
    free(temp);
}
/*
    @brief
    @param
    @return
*/
void skipComment(Scanner *self)
{
    // eat(self);
    // char *temp = findStar(self, "\n");
    // if (temp != NULL) {
    //     free(temp);
    // }
    // else {
    //     error(self, "Error: No newline found in comment");
    // }
    // free(temp);
    // eat(self);
    eat(self); // Move past the comment character (assuming it's // or #)
    while (currentCh(self) != '\n' && !atEOI(self)) {
        eat(self);
    }
    if (!atEOI(self)) {
        eat(self); // Move past the newline character itself
    }

}
/*
    @brief
    @param
    @return
*/
void jump(Scanner *self)
{
    if (inSet(currentCh(self), WHITESPACE)) {
        skipWhitespace(self);
    } else if (currentCh(self) == START_COMMENT) {
        skipComment(self);
    }
}
/*
    @brief
    @param
    @return
*/
void jumpStar(Scanner *self)
{
    // while(inSet(currentCh(self), WHITESPACE) || currentCh(self) == START_COMMENT) {
    //     jump(self);
    // }
    while (inSet(currentCh(self), WHITESPACE) || currentCh(self) == START_COMMENT) {
        if (inSet(currentCh(self), WHITESPACE)) {
            skipWhitespace(self);
        } else if (currentCh(self) == START_COMMENT) {
            skipComment(self);
        }
    }

}

//------------------------------
// C. High level tokenizers
//------------------------------
/*
    @brief
    @param
    @return
*/
Token NUM(Scanner *self)
{
    Token t;
    t.token = strdup("numConstant");
    t.isNum = 1;        // sets flag to 1, thus lexeme is a number
    char *temp = skipStar(self, DIGITS);
    t.lexeme.num = atoi(temp);
    free(temp);
    return t;    
}


/*
    @brief
    @param
    @return
*/
// Token ID(Scanner *self)
// {
//     Token t;
//     t.token = strdup("identifier");
//     t.isNum = 0;
//     char *idStr = skipStar(self, LETTERS); 
//     char buffer[256];
//     strcpy(buffer, idStr);
//     free(idStr);
//     char *rest = skipStar(self, DIGITS);
//     strcat(buffer, rest);
//     free(rest);
//     t.lexeme.str = strdup(buffer);
//     return t;
// }
Token ID(Scanner *self)
{
    Token t;
    t.token = strdup("identifier");
    t.isNum = 0;

    char buffer[256]; // Buffer to store identifier
    int idx = 0;

    // The first character must be a letter
    buffer[idx++] = currentCh(self);
    eat(self);

    int prevUnderscore = 0; // Flag to track consecutive underscores

    // Continue parsing while the character is a letter, number, or underscore
    while (inSet(currentCh(self), LETTERS_DIGITS) && !atEOI(self))
    {
        char ch = currentCh(self);

        if (ch == '_')
        {
            if (prevUnderscore)
            {
                error(self, "Invalid identifier: Consecutive underscores are not allowed.");
                break;
            }
            prevUnderscore = 1; // Set flag if underscore found
        }
        else
        {
            prevUnderscore = 0; // Reset flag if it's not an underscore
        }

        buffer[idx++] = ch;
        eat(self);
    }

    // If the last character was an underscore, it's invalid
    if (buffer[idx - 1] == '_')
    {
        error(self, "Invalid identifier: Cannot end with an underscore.");
        return (Token){NULL, {NULL}, 0}; // Return invalid token
    }

    buffer[idx] = '\0'; // Null-terminate the identifier
    t.lexeme.str = strdup(buffer);

    return t;
}


/*
    @brief
    @param
    @return
*/
Token STRI(Scanner *self)
{
    Token t;
    t.token = strdup("stringConstant");
    t.isNum = 0;
    eat(self);
    char *temp = find(self, END_STRING);
    eat(self);
    if (temp != NULL) {
        t.lexeme.str = strdup(temp);
        free(temp);
    }
    else {
        error(self, "Error: No end string found");
    }
    return t;

}
/*
    @brief
    @param
    @return
*/
char *twoCharSym(Scanner *self, char secondCh, const char *firstToken, const char *secondToken)
{
    eat(self);
    if (currentCh(self) == secondCh) {
        eat(self);
        return strdup(secondToken);
    } else {
        return strdup(firstToken);
    }
}
/*
    @brief
    @param
    @return
*/
Token nextToken(Scanner *self)
{
    Token t;
    t.token = NULL;
    t.lexeme.str = NULL;
    t.isNum = 0;

    if(atEOI(self)) {
        t.token = strdup(eoIToken);
        return t;
    }

    jumpStar(self);

    char currentChar = currentCh(self);

    if (inSet(currentChar, DIGITS))
    {
        return NUM(self);
    }
    
    if (inSet(currentChar, "'"))
    {
        return STRI(self);
    } 
    
    if (inSet(currentChar, LETTERS))
    {
        Token tokenId = ID(self);
        for(int i = 0; i < KEYWORD_TABLE_SIZE; i++)
        {
            if(strcmp(tokenId.lexeme.str, KEYWORD_TABLE[i].keyword) == 0)
            {
                free(tokenId.token);
                free(tokenId.lexeme.str);
                t.token = strdup(KEYWORD_TABLE[i].token);
                t.lexeme.str = NULL;
                return t;
            }
        }
        return tokenId;
    }

    // Two-char tokens: ==, !=, >=, <=
    if(currentChar == EQUAL)
    {
        char *sym = twoCharSym(self, EQUAL, "assignSym", "equalSym");
        t.token = sym;
        t.lexeme.str = NULL;
        return t;
    }
    if(currentChar == NOT)
    {
        char *sym = twoCharSym(self, EQUAL, "notSym", "notEqualSym");
        t.token = sym;
        t.lexeme.str = NULL;
        return t;
    }
    if(currentChar == GREATER)
    {
        char *sym = twoCharSym(self, EQUAL, "greaterSym", "greaterEQSym");
        t.token = sym;
        t.lexeme.str = NULL;
        return t;
    }
    if(currentChar == LESS)
    {
        char *sym = twoCharSym(self, EQUAL, "lessSym", "lessEQSym");
        t.token = sym;
        t.lexeme.str = NULL;
        return t;
    }

    // One-char tokens: Check in Op table
    for(int i = 0; i < OP_TABLE_SIZE; i++)
    {
        if(currentChar == OP_TABLE[i].op)
        {
            eat(self);
            t.token = strdup(OP_TABLE[i].token);
            t.lexeme.str = NULL;
            return t;
        }
    }

    // shrug it off, no idea!
    t.token = NULL;
    t.lexeme.str = NULL;

    return t;
}
