/*
 *      PROJECT-4
 *      KAAN GÜNER
 *      220201068
 */


#include <stdio.h>  // Library for standard input/output operations
#include <stdlib.h> // Library for standard functions
#include <string.h> // Library for string handling functions
#include <ctype.h>  // Library for character classification functions

// Global array to store lexicon words
char lexicon[1500][21]; // A 2D array to store up to 1500 words, each with a maximum length of 19 characters plus a null terminator.
int lexicon_size = 0; // Keeps track of the number of words in the lexicon.

// Function to check if a word is in the lexicon
int is_in_lexicon(const char *word) {
    // Check each word in the lexicon array
    for (int i = 0; i < lexicon_size; i++) {
        if (strcmp(lexicon[i], word) == 0) { // Check if the word matches
            return 1; // Word is found in the lexicon
        }
    }
    return 0; // Word is not found
}

// Function to check the input file and report errors
void check_file(const char *input_filename, const char *output_filename) {
    FILE *input_file = fopen(input_filename, "r"); // Open the input file in read mode
    FILE *output_file = fopen(output_filename, "w"); // Open the output file in write mode

    char word[20]; // Temporary storage for words
    int line = 1, column = 1;   // Line and column tracking

    // Read the input file word by word
    while (fscanf(input_file, "%49s", word) == 1) {
        // Report an error if the word is not in the lexicon
        if (!is_in_lexicon(word)) {
            fprintf(output_file, "Incorrect word '%s' found at line %d, column %d\n", word, line, column);
            fprintf(output_file, "Possible suggestions for '%s':\n", word);

            // Check each word in the lexicon
            for (int i = 0; i < lexicon_size; i++) {
                int len_diff = abs((int)strlen(word) - (int)strlen(lexicon[i])); // Calculate length difference

                // Consider words with a small length difference
                if (len_diff <= 1) {
                    int mismatch_count = 0; // Reset mismatch counter

                    // Compare characters between the two words
                    for (int j = 0; j < strlen(word) && j < strlen(lexicon[i]); j++) {
                        if (word[j] != lexicon[i][j]) { // If characters do not match
                            mismatch_count++; // Increment mismatch counter
                        }
                    }

                    // Print suitable suggestions
                    if (mismatch_count <= 1) {
                        fprintf(output_file, "- %s\n", lexicon[i]);
                    }
                }
            }
        }

        // Advance the column position
        column += strlen(word) + 1;

        // Check for end-of-line characters and update line/column information
        int ch;
        while ((ch = fgetc(input_file)) != EOF && isspace(ch)) {
            if (ch == '\n') { // If a newline character is found
                line++;      // Increment the line number
                column = 1;  // Reset the column position
            } else {
                column++; // Increment the column position for space characters
            }
        }

        // Put the last read character back into the input stream
        if (ch != EOF) {
            ungetc(ch, input_file);
        }
    }

    fclose(input_file); // Close the input file
    fclose(output_file); // Close the output file
}

int main() {
    FILE *file = fopen("lexicon.txt", "r"); // Open the lexicon file in read mode

    // Read each word from the file and add it to the lexicon array
    while (fscanf(file, "%49s", lexicon[lexicon_size]) == 1) {
        lexicon_size++; // Increment the word count
    }

    fclose(file); // Close the file

    check_file("input_text.txt", "output_file.txt"); // Check the input file

    printf("Done"); // Print a message indicating the process is complete

    return 0; // Program terminated successfully
}
