#include <stdio.h>
#include "lexer.h"
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "stdbool.h"

void prompt();
char *envConvert(char *item);
char *strdup(const char *s);
char *pathSearch(char* item);
void extcmd(tokenlist* itemlist);
void pipeFunc(char ***listOfList, int cmdCtr);

void extcmd(tokenlist* itemlist){
    
    bool pipeExists = false; //if loop contains pipe
    int pipeCounter = 0; //how many pipes does it contain (we only need the counter but I am still using pipeExist).
    int commandCounter = 0; //how many commands   

    for(int i = 0; i < itemlist->size; i++)
    {   
        if(itemlist->items[i][0] == '|')//check the token list for a pipe command
        {
            pipeExists = true; //true if  it does

            if(i != ((itemlist->size) - 1))
            {
                pipeCounter = pipeCounter + 1; //increase counter by one for every pipe if it isn't the final pipe           
            }
        }
    }

    if(pipeExists != true) //if pipe doesn't exist, run Rod's original function
    {
        int status;
        pid_t pid = fork();
        // printf("PID: %d\n", pid);

        if (pid == 0) {
            execv(itemlist->items[0], itemlist->items);
        }
        else {
                waitpid(pid, &status, 0);
            //    printf("Child Complete\n");
        }
    }

    if(pipeExists == true) //we need to separate every single command and argument from the tokens.
    {
        commandCounter = pipeCounter + 1;
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

        /*for(int i = 0; i < listListIdx; i++) //check if commands are coming out correctly
        {
           printf("Command %d: \n", i+1);
           for(int j = 0; listOfToList[i][j] != NULL; j++)
           {
              printf("\t%s\n", listOfToList[i][j]);
           }
        }*/ //I have commented out this test code, if you want to check that commands are being set correctly, uncomment this

        pipeFunc(listOfToList, commandCounter); //use the piping function on the list of commands

        for(int i = 0; i < commandCounter; i++) //free the allocated mem
        {
            free(listOfToList[i]);
        }
        
        free(listOfToList); //free
    }
}


int main()
{
    char *input;
    tokenlist *tokens;
    int pipeIndex;

    while(1)
    {
        pipeIndex = -1;
        prompt();
        
        input = get_input();
        //printf("The input is: %s\n", input); //I have commented input out since it seems unnecessary at this point. Uncomment if you need.                                               

        tokens = get_tokens(input);
       
        for(int i = 0; i < tokens->size; i++)
        {
            if(tokens->items[i][0] == '$') //if its an env variable
            {
                char *envString =  envConvert(tokens->items[i]); //convert it using func
                
                if(envString)
                {
                    free(tokens->items[i]); //free in order to dup
                    tokens->items[i] = strdup(envString); //dup to keep getenv safe
                }
            }
            else if(tokens->items[i][0] == '|') //was missing the 2d array, was looking for this for a good 30 mins
            {
                 pipeIndex = i;
            }
            else if(tokens->items[i][0] == '~') //if its tilde
            {
                if(tokens->items[i][1] == '/') //check if they have tilde alone or followed by a slash
                {
                    char *envString = getenv("HOME"); //get $HOME ready
                    char *envString1 = strdup(envString); 
                    char *envString2 = ((tokens->items[i]) + 1); //move forward once to get rid of the tilde but keep the slash and users path
                    
                    size_t length = strlen(envString1) + strlen(envString2) + 1; //find length
                    
                    char *envString3 = (char *) (malloc(length)); //allocate space on envString3
                    strcpy(envString3, envString1); //copy $HOME
                    char *newString = strcat(envString3, envString2); //concactenate

                    if(newString)
                    {
                        tokens->items[i] = strdup(newString); //duplicate to keep getenv return safe
                    }

                    free(envString1); //free memory
                    free(envString3);
                }
                else
                {
                    char *envString = getenv("HOME"); //if not followed by slash, just replace tilde token with $HOME
                    
                    if(envString)
                    {
                        free(tokens->items[i]); //free tilde
                        tokens->items[i] = strdup(envString); //dup to keep getenv safe
                    }       
                }                
            }

            
            //check for commands in their most common locations  
            if((i > 0) && ((i-1) == pipeIndex))
            {  
                tokens->items[i] = pathSearch(tokens->items[i]);
            }
            else if(i == 0){
                tokens->items[i] = pathSearch(tokens->items[i]);
            }
           
            

            //we will need to implement step 9 to check if the current item is an internal command before this is called
            //tokens->items[i] = pathSearch(tokens->items[i]);
        }  

        extcmd(tokens);
        
        /*for(int i = 0; i < tokens->size; i++)
        {
            printf("token %d: (%s)\n", i, tokens->items[i]);
        }*/ //I have commented out this test code. If you want to check if the tokens are being parsed correctly, uncomment this

        //free(input);
        //free_tokens(tokens);    
    }

    return 0;
}
