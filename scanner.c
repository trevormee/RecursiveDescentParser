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
    @brief Initializes the scanner with the input file and set up the initial state.
    @param(s): self - pointer to the Scanner struct
               source - the input source string file
    @return N/A
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
    @brief Helper function to check if a character is in a given set of characters.
    @param(s): ch - the character to check
               set - the set of characters to check against
    @return 1 if the character is in the set, 0 otherwise
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
    @brief Prints an error message
    @param (s): self - pointer to the Scanner struct
               message - the error message to print
    @return N/A
*/
void error(Scanner *self, const char *message)
{
    printf(">>> Error: %s\n", message);
}


/*
    @brief Gets the current character in the source string
    @param self - pointer to the Scanner struct
    @return current character
*/
char currentCh(Scanner *self)
{
    return self->source[self->position];
}


/*
    @brief Moves the scanner to the next character
    @param self - pointer to the Scanner struct
    @return N/A
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
    @brief Checks if the scanner has reached the end of input 
    @param self - pointer to the Scanner struct
    @return 1 if at end of input
*/
int atEOI(Scanner *self)
{
    return currentCh(self) == EOI;
}

//------------------------------
// B. Second level movements
//------------------------------

/*
    @brief Same as move, but report error of moving past EOI
    @param self - pointer to the Scanner struct
    @return N/A
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
    @brief Finds the first occurrence of a character in the input stream,
           reports eorror if none found before EOI, collects all characters
           found in result string
    @param(s): self - pointer to the Scanner struct
               x - the character to find
    @return a string containing all characters found before x
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
        sprintf(errMsg, "EOI detected searching for %c", x);
        error(self, errMsg);
        return strdup(result);
    } else {
        return strdup(result);
    }
}


/*
    @brief Same as find but look for any character in the set s
    @param(s): self - pointer to the Scanner struct
               s - the set of characters to find
    @return a string containing all characters found before any character in s
*/
char *findStar(Scanner *self, const char *s)
{
    char result[256] = {0};
    int index = 0;
    while (!inSet(currentCh(self), s) && !atEOI(self)) {
        result[index++] = currentCh(self);
        result[index] = '\0';
        //move(self);
        eat(self);
    }
    if (atEOI(self)) {
        char errMsg[256];
        sprintf(errMsg, "EOI detected searching for %s", s);
        error(self, errMsg);
        return strdup(result);
    } else {
        return strdup(result);
    }
}


/*
    @brief Run over character "x" down the input stream, 
           report error if none other found before EOI,
           collect all characters found in a result string
    @param (s): self - pointer to the Scanner struct
               x - the character to skip
    @return a string containing all characters found before x
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
    @brief Same as skip, but for a set s
    @param(s): self - pointer to the Scanner struct
               s - the set of characters to find
    @return a string containing all characters found before any character in s
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
    @brief Skip over all whitespace
    @param self - pointer to the Scanner struct
    @return N/A
*/
void skipWhitespace(Scanner *self)
{
    char *temp = skipStar(self, WHITESPACE);
    free(temp);
}


/*
    @brief Skip over everything up to end of comment
    @param self - pointer to the Scanner struct
    @return N/A
*/
void skipComment(Scanner *self)
{
    if (currentCh(self) == START_COMMENT) {
        eat(self);
    }

    while (currentCh(self) != '\n' && !atEOI(self)) {
        eat(self);
    }

    if (!atEOI(self) && currentCh(self) == '\n') {
        eat(self);
    }
}



/*
    @brief Move over whiespace or comment (basd on current character)
    @param self - pointer to the Scanner struct
    @return N/A
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
    @brief Move over whitespace or comment 
    @param self - pointer to the Scanner struct
    @return N/A
*/
void jumpStar(Scanner *self)
{
    while(inSet(currentCh(self), WHITESPACE) || currentCh(self) == START_COMMENT) {
        jump(self);
    }
}

//------------------------------
// C. High level tokenizers
//------------------------------

/*
    @brief Skip over digits and return a string of digits
    @param self - pointer to the Scanner struct
    @return a string of digits
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
Token ID(Scanner *self)
{
    Token t;
    t.token = strdup("identifier");
    t.isNum = 0;

    char result[256]; 
    int index = 0;

    result[index++] = currentCh(self);
    eat(self);

    int prevUnderscore = 0; // Flag to track consecutive underscores

    while (inSet(currentCh(self), LETTERS_DIGITS) && !atEOI(self))
    {
        char ch = currentCh(self);

        if (ch == '_')
        {
            if (prevUnderscore)
            {
                error(self, "Syntax Error! Consecutive underscores not allowed.");
                break;
            }
            prevUnderscore = 1; // Set flag if underscore found
        }
        else
        {
            prevUnderscore = 0; // Reset flag if it's not an underscore
        }

        result[index++] = ch;
        eat(self);
    }

    if (result[index - 1] == '_')
    {
        error(self, "Invalid identifier: Cannot end with an underscore.");
        return (Token){NULL, 0, 0};
    }

    result[index] = '\0'; 
    t.lexeme.str = strdup(result);

    return t;
}


/*
    @brief Skips over everything up to the end of the string
    @param self - pointer to the Scanner struct
    @return scanned string
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
    @brief Scans a possibly two-character token (e.g., ==, !=, >=, <=)
    @param self - pointer to the Scanner struct
           secondCh - the second character to check for
           firstToken - the token to return if secondCh is not found
           secondToken - the token to return if secondCh is found
    @return a string containing the token
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
    @brief Scans the next token in the input stream
           and returns a Token struct containing the token class
           and lexeme.
    @param self - pointer to the Scanner struct
    @return class and leeme of the next token
*/
Token nextToken(Scanner *self)
{
    Token t;
    t.token = NULL;
    t.lexeme.str = NULL;
    t.isNum = 0;

    // Trivial test of EOI
    if(atEOI(self)) {
        t.token = strdup(eoIToken);
        return t;
    }

    // Find next token start
    jumpStar(self);

    // Check the character at the current position
    char currentChar = currentCh(self);

    if(currentChar == START_COMMENT)
    {
        skipComment(self);
    }

    // Start all of the possibilities (hopefully)

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
