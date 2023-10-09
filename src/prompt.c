#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void prompt()
{
    char buffer[200]; 
    if(!getcwd(buffer, sizeof(buffer)))
    {
        printf("\nError with directory!\n");
    }   
    else
    {
        printf("%s@%s:%s>", getenv("USER"), getenv("MACHINE"), buffer);
    }
}
