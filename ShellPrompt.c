#include <stdlib.h>
#include <stdio.h>

int main()
{
    char *user = getenv("USER");
    char *mach = getenv("MACHINE");
    char *pwd = getenv("PWD");

    printf("%s@%s:%s>", user, mach, pwd);
    
}
