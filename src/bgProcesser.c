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
        //Seg fault from if condition 
        if((itemlist->items[i] != NULL) && (itemlist->items[i][0] == '|')){
            //check the token list for a pipe command
            pipeExists = true; //true if  it does

            if(i != ((itemlist->size) - 1))
            {
                pipeCounter = pipeCounter + 1; //increase counter by one for every pipe if it isn't the final pipe           
            }
        }
    }

    if (pid == 0) {
        ioRedirection(itemlist);

        //if pipes exist, use listOfCommands function and use piping function
        if(pipeExists == true) 
        {
            commandCounter = pipeCounter + 1; //get command counter

            char ***listOfCommands = listList(itemlist, pipeCounter); //get list of commands
            printf("[%d][%d]", jobNum, pid);
            //do piping for the commands
            //(not running in the background as I expect it to do here)
            pipeFunc(listOfCommands, commandCounter, true); 

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
            printf("[%d] + done [look up above]\n", jobNum);
            /*for(int j = 0; j < (BG[jobNum].itemlist->size); j++)
                if(BG[jobNum].itemlist->items[j] != NULL){
                    printf("%s ", BG[jobNum].itemlist->items[j]);
                }
            printf("]\n");*/
        }
    }
}
