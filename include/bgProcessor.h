#include <stdio.h>
#include "lexer.h"
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "stdbool.h"


int jobNum = 0;

struct bgPid{
    pid_t pid;
    int jobNum;
    tokenlist* itemlist;
    bool isValid;
};
