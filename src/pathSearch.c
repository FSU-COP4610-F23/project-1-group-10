// #include "lexer.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

char* pathSearch(char* item)
{
    // if item contains a "/",
    char* slashInCommand = strstr(item, "/");
    if (slashInCommand != NULL) {
        // check accessibility of command at the given path and return it if it is accessible
        if (access(item, X_OK) == 0)
            return item;
        // if the path given is not executable, print error message and return nothing
        else {
            printf("Command [%s] not found\n", item); //changed some error handling
            return item; //return error to token
        }
    }


    char* directory; // to hold portion of path between ":"s
    char* proposedPath; // to hold the path being tested
    
    // DO NOT ALTER ORIGPATH; IT NOW POINTS TO $PATH GETENV RESULT
    char* origPath = getenv("PATH");
    // find out length of getenv result
    int pathLength = strlen(origPath) + 1;
    // dynamically declare path c-string of size pathLength
    char* path = (char*)calloc(pathLength, sizeof(char));
    // copy string value of $PATH into path, which won't point to $PATH
    strcpy(path, origPath);

    origPath = NULL;    // origPath no longer points to string value of $PATH, phew

    // set directory to the portion of path between its start to the first ":"
    directory = strtok(path, ":");
    // allocate memory to fit directory + / + command
    //proposedPath = (char*)calloc(strlen(directory) + sizeof(char) + strlen(item), sizeof(char));
    proposedPath = (char*)calloc(strlen(directory) + strlen(item) + 2, sizeof(char)); 
    //small changes with arrows are here to fix a mem leak
    int i = 0;
    // as long as directory continues to contain a portion of path,
    while(directory != NULL) {
        //change the size of proposedPath to fit the new directory + / + command,
        
        //proposedPath = (char*)realloc(proposedPath, (strlen(directory)
        // + sizeof(char) + strlen(item)) * sizeof(char));
        char* newProposedPath = realloc(proposedPath, strlen(directory) + strlen(item) + 2); 
        proposedPath = newProposedPath; 

        //free(newProposedPath); //<------

        // clear whatever proposedPath currently contains
        //memset(proposedPath, 0, strlen(proposedPath));
        memset(proposedPath, 0, strlen(directory) + strlen(item) + 2); //<-----
        // copy the c-string in directory into proposedPath
        strcpy(proposedPath, directory);
        // append a "/" + the command to that
        strcat(proposedPath, "/");
        strcat(proposedPath, item);
        // return the current value of proposedPath if the command can be accessed
        // for execution at that location
        if (access(proposedPath, X_OK) == 0) {
            // free dynamically allocated memory
            free(item);
            free(path);
            return(proposedPath);
        }
        // replace directory with the next portion of path between ":"s
        directory = strtok(NULL, ":");
        i++;

    }
    // print error message and return nothing if the command was not found in
    // any $PATH directory
    printf("Command [%s] not found\n", item); //changed error handling
    // free dynamically allocated memory
    free(path);
    free(proposedPath);    
    
    return item; //makes it return error val
    
    free(item); //free last dyn all mem
}

