#include "lexer.h"
#include "bgProcessor.h"

void ioRedirection(tokenlist*);

void pipeFunc(char ***listOfList, int cmdCtr, bool bgp);
char ***listList(tokenlist* itemlist, int pipeCounter);

//This is the main part 8 func. 
void bgProcessing(tokenlist* itemlist, struct bgPid *BG){
    int status;
    int commandCounter = 0;
    int pipeCounter = 0;
    bool pipeExists = false;
    pid_t pid = fork();

    for(int i = 0; i < itemlist->size; i++) //has a pipe checker
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

    if (pid == 0) {
        ioRedirection(itemlist);

        if(pipeExists == true) //if pipes exist, use list of commands function and use piping function
        {
            commandCounter = pipeCounter + 1; //get command counter

            char ***listOfCommands = listList(itemlist, pipeCounter); //get list of commands
            printf("[%d][%d]", jobNum, pid);
            pipeFunc(listOfCommands, commandCounter, true); //do piping for the commands

            for(int i = 0; i < commandCounter; i++)
            {
                free(listOfCommands[i]); //free the used memory
            }   
    
            free(listOfCommands); //free
        }       
        else
        {
            printf("[%d][%d]", jobNum, pid);
            execv(itemlist->items[0], itemlist->items);
        }
    }
    else {
        //WNOHANG lets us continue w/o waiting on the child process
        waitpid(pid, &status, WNOHANG);
        BG[jobNum].pid = pid;
        BG[jobNum].jobNum = jobNum;
        BG[jobNum].itemlist = itemlist;
        BG[jobNum].isValid = true;
        jobNum++;
    }
}

//checks if and when background jobs finish
void checkBG(struct bgPid *BG, int size){
    int status;
    for(int i = 1; i < size; i++){
        waitpid(BG[i].pid, &status, WNOHANG);
        if((WIFEXITED(status) == true) && (BG[i].isValid == true)){
            printf("[%d] + done [", jobNum);
            for(int j = 0; j < ((BG[jobNum].itemlist->size) - 1); j++)
               printf("%s ", BG[jobNum].itemlist->items[j]);
            printf("]\n");
        }
    }
}

/*
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
*/
