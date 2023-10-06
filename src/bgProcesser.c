

//This is the main part 8 func. 
void bgProcessing(tokenlist* itemlist, struct bgPid *BG){
    int status;
    pid_t pid = fork();

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
    if (pid == 0) {
        if(pipeExists == true) //if pipes exist, use list of commands function and use piping function
        {
            commandCounter = pipeCounter + 1; //get command counter

            char ***listOfCommands = listList(tokens, pipeCounter); //get list of commands
            pipeFunc(listOfCommands, commandCounter, false); //do piping for the commands

            for(int i = 0; i < commandCounter; i++)
            {
                free(listOfCommands[i]); //free the used memory
            }   
    
            free(listOfCommands); //free
        }
        printf("[%d][%d]", jobNum, pid);
        execv(itemlist->items[0], itemlist->items);
        printf("there\n");
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
