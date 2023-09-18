#include <stdlib.h>
#include <stdio.h>

char *envConvert(char *item) 
{
    char *newEnv = item;

    if(item != NULL)
    {
        newEnv = getenv(item + 1);
    }

    return newEnv;
}
