#include "scanner.h"

static FILE *sourceFile; // Pointer to the source file
static int currentChar; // Current position in the source file
static int line; // Current line number

/*
    @brief Initializes the scanner with the given source file.
    @param fileName The name of the source file to be scanned.
    @return N/A
*/
void initScanner(const char *fileName) {
    // Open the source file
    sourceFile = fopen(fileName, "r");
    if (!sourceFile) {
        fprintf(stderr, "Error: Could not open file %s\n", fileName);
        exit(EXIT_FAILURE);
    }
    // Initialize scanner state
    currentChar = fgetc(sourceFile);
    line = 1;
    printf("Scanner initialized with file: %s\n", fileName);
    printf("Current character: %c\n", currentChar);
}