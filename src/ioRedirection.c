#include "lexer.h"
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

// For use inside child process created in part 5
// DANGEROUS TO RUN THIS OUTSIDE OF A CHILD PROCESS

void ioRedirection(tokenlist* tokens) {
    // declare c-strings to hold names of input and output files
    char* file_in;
    char* file_out;
    // loop through each token of input from the user up until the second to last
    int i = 0;
    while (i < tokens->size - 1) {
        // if the current token starts with "<"
        if (tokens->items[i][0] == '<') {
            // declare integer for holding the descriptor returned by open()
            int openDesc;
            // set file_in to the next token
            file_in = tokens->items[i+1];

            // open the file with the name in file_in for reading only
            openDesc = open(file_in, O_RDONLY, S_IRUSR);
            // if this fails, print an error message to stderr
            if (openDesc == -1)
                printf("Input file access failed \n");
            // if it was a success,
            else {
                // close stdin, which is at file descriptor 0
                close(0);
                // duplicate the file at the descriptor returned by open(), which will make a copy of file_in at descriptor 0
                dup(openDesc);
                // close the file at the descriptor returned by open(), which will remove file_in from descriptor 3
                close(openDesc);
                // the input file is now set to the file of the name in file_in
                //set the current item to NULL so that execv stops at the null and doesn't get confused if it comes across a <
                tokens->items[i] = NULL;
            }
        }
        // if the current token starts with ">"
        if (tokens->items[i][0] == '>') {
            // declare integer for holding the descriptor returned by open()
            int openDesc;
            // set file_out to the next token
            file_out = tokens->items[i+1];
            // open the file of the name in file_out for reading and writing, create it, or clear it if it exists
            openDesc = open(file_out, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
            // if this fails, print an error message to stderr
            if (openDesc == -1)
                printf("Output file access failed \n");
            // if it was a success,
            else {
                // close stdout, which is at file descriptor 1
                close(1);
                // duplicate the file at the descriptor returned by open(), which will make a copy of file_out at descriptor 1
                dup(openDesc);
                // close the file at the descriptor returned by open(), which will remove file_out from descriptor 3
                close(openDesc);
                // the output file is now set to the file of the name in file_out
                //set the current item to NULL so that execv stops at the null and doesn't get confused if it comes across a >
                tokens->items[i] = NULL;
            }
        }
        i++;
    }
}
