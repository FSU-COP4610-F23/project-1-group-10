#include <stdio.h>
#include "lexer.h"
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "stdbool.h"


//Global variable for each shell process
//int jobNum = 1;
struct bgPid{
    pid_t pid;
    int jobNum;
    tokenlist* itemlist;
    bool isValid;
};

char *history[3] = {NULL, NULL, NULL};
//make a list of the commands for history


void prompt();
char *envConvert(char *item);
char *strdup(const char *s);
char *pathSearch(char* item);
void extcmd(tokenlist* itemlist);
void ioRedirection(tokenlist*);
void pipeFunc(char ***listOfList, int cmdCtr, bool bgp);
char ***listList(tokenlist* itemlist, int pipeCounter);
void bgProcessing(tokenlist* itemlist, struct bgPid *BG);
void checkBG(struct bgPid *BG, int size);
void addHistoryFunc(const char *command);
void exitFunc(struct bgPid *bg, int size); 
void cdFunc(tokenlist *tokens);
void jobsFunc(struct bgPid *bg, int size);



void AddHistoryFunc(const char *command) 
{
    if(history[2])
    {
        free(history[2]); //free last spot 
    }

    for(int i = 2; i > 0; i--) //replace 
    {
        history[i] = history[i - 1];
    }

    history[0] = strdup(command);
}

void exitFunc(struct bgPid *bg, int size)
{
    for(int i = 0; i < size; i++)
    {
        if(bg[i].isValid)
        {
            waitpid(bg[i].pid, NULL, 0);
        }
    } //check if active

    for(int i = 0; i < 3; i++)
    {
        if(history[i])
        {
            printf("%s\n", history[i]);
        }
    } //print last 3

    exit(0);
}

void cdFunc(tokenlist *tokens)
{
    if(tokens->size > 2)
    {
        printf("\nThere is more than one argument.\n");
        return;
    }//more than one arg
    
    char *newPath;

    if(tokens->size == 1)
    {
        newPath = getenv("HOME");
    } //if just cd, go home
    else
    {
        newPath = tokens->items[1];
    }//if more than just cd, newPath is second item

    if(chdir(newPath) != 0) //try to change dir
    {
        printf("\nThis is not a path!\n");
    }
}   

void jobsFunc(struct bgPid *bg, int size)
{
    bool jobsExist = false;

    for(int i = 0; i < size; i++)
    {
        if(bg[i].isValid)
        {
            printf("[%d]+ %d %s\n", bg[i].jobNum + 1, bg[i].pid, bg[i].itemlist->items[0]);
            jobsExist = true;
        }
        //check if there are jobs
        

        /*if(!jobsExist)
        {
            printf("No active background processes.\n");
        }*/
    }
    if(jobsExist == false)
    {
        printf("\nNo active background processes.\n");
    }
}

void extcmd(tokenlist* itemlist){
    int status;
    int pipeCounter = 0;
    int commandCounter = 0;
    bool pipeExists = false;
    pid_t pid = fork();
    // printf("PID: %d\n", pid);

    for(int i = 0; i < itemlist->size; i++) //has a pipe checker
        {   
            if(itemlist->items[i][0] == '|')//check the token list for a pipe command
            {
                pipeExists = true; //true if  it does

                if(i != ((itemlist->size) - 1))
                {
                    pipeCounter = pipeCounter + 1; 
                    //increase counter by one for every pipe if it isn't the final pipe           
                }
            }
        }
        
    if (pid == 0) {
        ioRedirection(itemlist);

        if(pipeExists == true) 
        //if pipes exist, use list of commands function and use piping function
        {
            commandCounter = pipeCounter + 1; //get command counter

            char ***listOfCommands = listList(itemlist, pipeCounter); //get list of commands
            pipeFunc(listOfCommands, commandCounter, false); //do piping for the commands

            for(int i = 0; i < commandCounter; i++)
            {
                free(listOfCommands[i]); //free the used memory
            }   
    
            free(listOfCommands); //free
        }
        else
        {
            execv(itemlist->items[0], itemlist->items);
        }
    }
    else {
        waitpid(pid, &status, 0);
        //    printf("Child Complete\n");
    }
}

int main()
{
    bool isCD = false;
    char *input;
    tokenlist *tokens;
    bool intCommand = false;
    int pipeIndex;
    struct bgPid bg[11];
    for(int i = 0; i<11; i++)
        bg[i].isValid = false;
    

    while(1)
    {
      //  bool pipeExists = false; //if loop contains pipe
      //  int pipeCounter = 0; 
        //how many pipes does it contain
      //  int commandCounter = 0; //how many commands
        pipeIndex = -1;
        prompt();
        
        input = get_input();
     //   printf("The input is: %s\n", input);

        checkBG(bg, 11);

        tokens = get_tokens(input);

        if(strcmp(tokens->items[0], "exit") == 0)
        {
            //do exit
            intCommand = true;
            exitFunc(bg, 11);
        }
        else if(strcmp(tokens->items[0], "cd") == 0)
        {
            //check for cd
            intCommand = true;
            isCD = true;
            //cdFunc(tokens);
        }
        else if(strcmp(tokens->items[0], "jobs") == 0)
        {
            //check for jobs
            intCommand = true;
            jobsFunc(bg, 11);
        }
        
       
        for(int i = 0; i < tokens->size; i++)
        {
            if(tokens->items[i][0] == '$')
            {
                char *envString =  envConvert(tokens->items[i]);
                //use env convert to get env var
                if(envString)
                {
                    free(tokens->items[i]);
                    tokens->items[i] = strdup(envString);
                } //make space and dup
            }
            else if(tokens->items[i][0] == '|') pipeIndex = i;
            //check if has pipes
            else if(tokens->items[i][0] == '~')
            {
            //check if tilde, and if it has backslash
                if(tokens->items[i][1] == '/')
                {
                    char *envString = getenv("HOME");
                    //get home
                    char *envString1 = strdup(envString); 
                    char *envString2 = ((tokens->items[i]) + 1);
                    //remove ~
                    size_t length = strlen(envString1) + strlen(envString2) + 1;
                    //make space
                    char *envString3 = (char *) (malloc(length));
                    strcpy(envString3, envString1);
                    char *newString = strcat(envString3, envString2);
                    //join together
                    if(newString)
                    {
                        tokens->items[i] = strdup(newString);
                    }
                    //replace token
                    free(envString1);
                    free(envString3);
                }
                else
                {
                    char *envString = getenv("HOME");
                    
                    if(envString)
                    {
                        //much easier, we just have home
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
            // we will need to implement step 9 to check if the 
            // current item is an internal command before this is called
            //tokens->items[i] = pathSearch(tokens->items[i]);
        }  
            
         //   printf("Show the token size here: %d\n", tokens->size);
            if(tokens->size > 0){
                if(tokens->items[(tokens->size)-1][0] == '&'){
                    if(intCommand == false)
                    {
                        tokens->items[(tokens->size)-1] = NULL;
                        printf("Going to the background\n");
                        bgProcessing(tokens, bg);
                    }
                }
                else
                { 
                    if(intCommand == false)
                    {
                        extcmd(tokens);
                    }
                }
                for(int i = 0; i < tokens->size; i++)
                    printf("token %d: (%s)\n", i, tokens->items[i]);

//                free_tokens(tokens);    
            }
        
        if(isCD == true && intCommand == true)
        {
            cdFunc(tokens);
        }
        //in order to process paths before cd, use this

        isCD = false;//reset for next loop
        intCommand = false;

        free(input);
    }

    return 0;
}
