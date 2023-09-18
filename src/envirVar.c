#include <stdlib.h>
#include <stdio.h>

char *envConvert(char *item) 
{
    char *newEnv = item;

    if(item != NULL)
    {
        if(*item[0] == '~'){
    /*        char* temp1 = getenv("HOME");
            int size = strlen(item) + strlen(temp1) - 1;
            char* newitem = (char*) malloc(size * sizeof(char))
            for(int i = 0; i < size; i++){
                if(i < strlen(temp1)){ newitem[i] = temp1[i]; }
                else{ newitem[i] = item[i - strlen(temp1) + 1]; }
            }
            for(int i = 0; i < strlen(item); i++)*/
            
            newEnv = getenv("HOME");
        }
        else{ newEnv = getenv(item + 1); }
    }

    return newEnv;
}
