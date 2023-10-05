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
char ***listList(tokenlist* itemlist, int pipeCounter);

void extcmd(tokenlist* itemlist){

    int status;
    pid_t pid = fork();
    // printf("PID: %d\n", pid);

    if (pid == 0) {
         execv(itemlist->items[0], itemlist->items);
    }
    else {
         waitpid(pid, &status, 0);
        // printf("Child Complete\n");
    }    
}


int main()
{
    char *input;
    tokenlist *tokens;
    int pipeIndex;

    while(1)
    {
        bool pipeExists = false; //if loop contains pipe
        int pipeCounter = 0; //how many pipes does it contain (we only need the counter but I am still using pipeExist).
        int commandCounter = 0; //how many commands

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

        for(int i = 0; i < tokens->size; i++) //has a pipe checker
        {   
            if(tokens->items[i][0] == '|')//check the token list for a pipe command
            {
                pipeExists = true; //true if  it does

                if(i != ((tokens->size) - 1))
                {
                    pipeCounter = pipeCounter + 1; //increase counter by one for every pipe if it isn't the final pipe           
                }
            }
        }
        
        if(pipeExists != true) //if pipes do not exist, run normally
        {
            extcmd(tokens);
        }

        if(pipeExists == true) //if pipes exist, use list of commands function and use piping function
        {
            commandCounter = pipeCounter + 1; //get command counter

            char ***listOfCommands = listList(tokens, pipeCounter); //get list of commands
            pipeFunc(listOfCommands, commandCounter); //do piping for the commands

            for(int i = 0; i < commandCounter; i++)
            {
                free(listOfCommands[i]); //free the used memory
            }   
    
            free(listOfCommands); //free
        }

        /*for(int i = 0; i < tokens->size; i++)
        {
            printf("token %d: (%s)\n", i, tokens->items[i]);
        }*/ //I have commented out this test code. If you want to check if the tokens are being parsed correctly, uncomment this

        //free(input);
        //free_tokens(tokens);    
    }

    return 0;
}
