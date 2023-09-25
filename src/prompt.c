#include <stdlib.h>
#include <stdio.h>

void prompt()
{
    printf("%s@%s:%s>", getenv("USER"), getenv("MACHINE"), getenv("PWD"));
}
