#include <asm-generic/errno-base.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "utility.h"


pid_t Fork(void)
{
    pid_t pid;

    if ((pid = fork()) < 0)
        unix_error("Fork error");
    return pid;
}

