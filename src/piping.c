#include <stdio.h>
#include "lexer.h"
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "stdbool.h"



void pipeFunc(char ***listOfList, int cmdCtr) //take in the list of cmds and cmd counter
{
    int fileDescriptor[2]; //file descriptor
    int prev = -1; //store fileDescriptor[0] from the last pipe
    int status; //status for waitpid
    pid_t pids[cmdCtr]; //pid of child

    for(int i = 0; i < cmdCtr; i++) //for every command, start the pipes
    {
        if(i == (cmdCtr - 1)) //check if its the last command
        {
            pid_t pid = fork(); //fork the parent process
            pids[i] = pid;

            if(pid == 0)
            {
                if(prev != -1)
                {
                    close(STDIN_FILENO); //use fileDescriptor[0] from last pipe as input
                    dup(prev);
                    close(prev);
                }
                
                execv(listOfList[i][0], listOfList[i]); //execute
                exit(1); //failure
            }
            
            if(prev != -1) //parent closes read end of last pipe
            {
                close(prev);
            }
        }
        else if(i == 0) //first command
        {
            pipe(fileDescriptor);
            pid_t pid = fork();
            pids[i] = pid;

            if(pid == 0)
            {
                close(fileDescriptor[0]); //fileDescriptor[1] of current pipe as output
                close(STDOUT_FILENO);
                dup(fileDescriptor[1]);
                close(fileDescriptor[1]);

                execv(listOfList[i][0], listOfList[i]); //execute
                exit(1); //failure
            }

            close(fileDescriptor[1]); //parent closes write of current pipe
            prev = fileDescriptor[0];
        }
        else //if it isnt the last command, or first, keep going
        {
            pipe(fileDescriptor);
            pid_t pid = fork();
            pids[i] = pid;            
                
            if(pid == 0)
            {
                close(STDIN_FILENO);
                dup(prev);
                close(prev);

                close(fileDescriptor[0]);
                close(STDOUT_FILENO);
                dup(fileDescriptor[1]);
                close(fileDescriptor[1]);
                
                execv(listOfList[i][0], listOfList[i]); //execute
                exit(1); //failure
            }
            
            if(prev != -1)
            {
                close(prev);
            }

            close(fileDescriptor[1]);
            prev = fileDescriptor[0];
        }
    }

    for(int i = 0; i < cmdCtr; i++)
    {
        waitpid(pids[i], &status, 0); //waitpid for child process
    }
}
