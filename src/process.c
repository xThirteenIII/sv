#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utility.h"

#define MAXARGS 32

pid_t Fork(void) {
    pid_t pid;

    if ((pid = fork()) < 0)
        unix_error("Fork error");
    return pid;
}

void run_service(char *cmdline) {
    char *buf = malloc(sizeof(char) * strlen(cmdline) + 2); // +1 for '\n'
                                                            // +1 for '\0'
    char **argv = malloc(sizeof(char*) * MAXARGS); // allocate MAX space, to avoid bad stuff happening.
                                                   // No need to optimize for now
    strcpy(buf, cmdline);
    buf[strlen(buf)] = '\n'; // Add trailing newline. Useful for parsing.
    parseline(buf, argv);

    pid_t pid;
    int statuspd;

    pid = Fork();
    
    if (pid == 0) {
        printf("child executing: %s\n", argv[0]);
        if (execve(argv[0], argv, NULL) < 0 ) 
            unix_error("execve error\n"); // exit(EXIT_FAILURE) if execve returns 
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
    free(buf);
    free(argv);
}
