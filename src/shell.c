#include <stdio.h>
#include "lexer.h"
#include <string.h>

void prompt();
char *envConvert(char *item);
char *strdup(const char *s);

int main()
{
    char *input;
    tokenlist *tokens;

    while(1)
    {
        prompt();
        
        input = get_input();
        printf("The input is: %s\n", input);

        tokens = get_tokens(input);
       
        for(int i = 0; i < tokens->size; i++)
        {
            if(tokens->items[i][0] == '$' || tokens->items[i][0] == '~')
            {
                char *envString =  envConvert(tokens->items[i]);
                
                if(envString)
                {
                    free(tokens->items[i]);
                    tokens->items[i] = strdup(envString);
                }
            }
        }        
 
        for(int i = 0; i < tokens->size; i++)
        {
            printf("token %d: (%s)\n", i, tokens->items[i]);
        }

        free(input);
        free_tokens(tokens);    
    }

    return 0;
}
