#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "utility.h"
#include "process.h"

int main() {
    pid_t pid;
    int statuspd;
    char *buf = malloc(sizeof(char) * strlen("/usr/bin/sleep 2\n") + 1);
    int argc = 4;
    char **argv = malloc(sizeof(char*) * argc);
    strcpy(buf, "/usr/bin/sleep 2\n");
    parseline(buf, argv);

    pid = Fork();
    
    if (pid == 0) {
        printf("child executing: %s\n", argv[0]);
        if (execve(argv[0], argv, NULL) < 0 ) 
            unix_error("execve error\n"); /* exit(EXIT_FAILURE) if execve returns */
        exit(0);
    }

    pid_t child = waitpid(-1, &statuspd ,0);
    if (WIFEXITED(statuspd) && child > 0)
        printf("child %d terminated normally with exit status: %d\n", child, WEXITSTATUS(statuspd));
    else if (WIFSIGNALED(statuspd))
        printf("child %d terminated by signal %d\n", child, WTERMSIG(statuspd));
    else
        printf("child %d terminated abnormally\n", child);
    printf("Parent terminated child with PID: %d\n", child);
    return 0;
}
