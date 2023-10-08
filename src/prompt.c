#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void prompt()
{
    printf("%s@%s:%s>", getenv("USER"), getenv("MACHINE"), getenv("PWD"));

    //printf("\n%s>", getcwd());
}
