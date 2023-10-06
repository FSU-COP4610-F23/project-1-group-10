#include <stdio.h>
#include "lexer.h"
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "stdbool.h"

//Global variable for each shell process
int jobNum = 1;
struct bgPid{
    pid_t pid;
    int jobNum;
    tokenlist* itemlist;
    bool isValid;
};

void prompt();
char *envConvert(char *item);
char *strdup(const char *s);
char *pathSearch(char* item);
void extcmd(tokenlist* itemlist);
void pipeFunc(char ***listOfList, int cmdCtr);
char ***listList(tokenlist* itemlist, int pipeCounter);
void bgProcessing(tokenlist* itemlist, struct bgPid *BG);
void checkBG(struct bgPid *BG, int size);

void extcmd(tokenlist* itemlist){
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

int main()
{
    char *input;
    tokenlist *tokens;
    int pipeIndex;
    struct bgPid bg[11];
    for(int i = 0; i<11; i++)
        bg[i].isValid = false;

    while(1)
    {
        bool pipeExists = false; //if loop contains pipe
        int pipeCounter = 0; //how many pipes does it contain (we only need the counter but I am still using pipeExist).
        int commandCounter = 0; //how many commands
        pipeIndex = -1;
        prompt();
        
        input = get_input();
        printf("The input is: %s\n", input);

        checkBG(bg, 11);

        tokens = get_tokens(input);
       
        for(int i = 0; i < tokens->size; i++)
        {
            if(tokens->items[i][0] == '$')
            {
                char *envString =  envConvert(tokens->items[i]);
                
                if(envString)
                {
                    free(tokens->items[i]);
                    tokens->items[i] = strdup(envString);
                }
            }
            else if(tokens->items[i][0] == '|') pipeIndex = i;
            else if(tokens->items[i][0] == '~')
            {
                if(tokens->items[i][1] == '/')
                {
                    char *envString = getenv("HOME");
                    char *envString1 = strdup(envString); 
                    char *envString2 = ((tokens->items[i]) + 1);
                    
                    size_t length = strlen(envString1) + strlen(envString2) + 1;
                    
                    char *envString3 = (char *) (malloc(length));
                    strcpy(envString3, envString1);
                    char *newString = strcat(envString3, envString2);

                    if(newString)
                    {
                        tokens->items[i] = strdup(newString);
                    }

                    free(envString1);
                    free(envString3);
                }
                else
                {
                    char *envString = getenv("HOME");
                    
                    if(envString)
                    {
                        free(tokens->items[i]);
                        tokens->items[i] = strdup(envString);
                    }       
                }                
            }

            // check for commands in their most common locations  
            if((i > 0) && ((i-1) == pipeIndex)){
                tokens->items[i] = pathSearch(tokens->items[i]);
            }
            else if(i == 0){
                tokens->items[i] = pathSearch(tokens->items[i]);
            }
            // we will need to implement step 9 to check if the current item is an internal command before this is called
            //tokens->items[i] = pathSearch(tokens->items[i]);
        }  

        printf("Show the token size here: %d\n", tokens->size);
        if(tokens->size > 0){
            if(tokens->items[(tokens->size)-1][0] == '&'){
                tokens->items[(tokens->size)-1] = NULL;
                printf("Going to the background\n");
                bgProcessing(tokens, bg);
            }
            else extcmd(tokens);
            
            for(int i = 0; i < tokens->size; i++)
                printf("token %d: (%s)\n", i, tokens->items[i]);

            free_tokens(tokens);    
        }

        //free(input);
    }

    return 0;
}
