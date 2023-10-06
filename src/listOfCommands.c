#include <stdio.h>
#include "lexer.h"
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "stdbool.h"


char ***listList(tokenlist* itemlist, int pipeCounter)
{
    int commandCounter = pipeCounter + 1;
    char ***listOfToList = malloc((commandCounter) * sizeof(char**)); //allocate space for the commands list
    int listListIdx = 0; //which command are we writing to
    int tokenEndIdx = 0; //what token on the original list are we on
            
    for(int i = 0; i < itemlist->size; i++)
    {
        if(itemlist->items[i][0] == '|') //if there is a pipe
        {
            int sizeOfTokens = i - tokenEndIdx; //the amount of tokens between the current pipe and the first token of the cmd
            listOfToList[listListIdx] = malloc((sizeOfTokens + 1) * sizeof(char*)); //allocate space to write cmd and arg to the command
            for(int j = 0; j < sizeOfTokens; j++)
            {
                listOfToList[listListIdx][j] = itemlist->items[tokenEndIdx + j]; //write to the command, starting from the first token
            }
            listOfToList[listListIdx][sizeOfTokens] = NULL; //make sure to make the last token on the command null
                
            listListIdx = listListIdx + 1; //move to the next command
            tokenEndIdx = i + 1; //jump the beginning of the new command to the location at i + 1 to skip the pipe '|' char
        }
        else if(i == (itemlist->size - 1)) //the same as above, only difference is to account for the last token
        {
            int sizeOfTokens = i - tokenEndIdx + 1; //the amount of tokens between first cmd token (last pipe + 1) and the last token
            listOfToList[listListIdx] = malloc((sizeOfTokens + 1) * sizeof(char*));

            for(int j = 0; j < sizeOfTokens; j++)
            {
                listOfToList[listListIdx][j] = itemlist->items[tokenEndIdx + j];
            }
                listOfToList[listListIdx][sizeOfTokens] = NULL;
                
                listListIdx = listListIdx + 1;    
                tokenEndIdx = i + 1;
        }    
    }

    for(int i = 0; i < listListIdx; i++)
    {
        printf("Command %d: \n", i+1);
        for(int j = 0; listOfToList[i][j] != NULL; j++)
        {
            printf("\t%s\n",
            listOfToList[i][j]);
        }
    }

    return listOfToList;
}
