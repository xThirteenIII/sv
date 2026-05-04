#include <asm-generic/errno-base.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
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

    // Ctrl+C kills both parent and child, thus I can't see if the child is being killed or not.
    // For testing, let's ignore the SIGINT in the parent, and put DEFAULT behaviour in the child.
    // This works.
    signal(SIGINT, SIG_IGN);
    pid = Fork();
    
    if (pid == 0) {
        printf("child executing: %s\n", argv[0]);
        signal(SIGINT, SIG_DFL);
        execv(argv[0], argv);  // execv over execve because we don't care about environ for now
        unix_error("execve error\n"); // exit(EXIT_FAILURE) if execve returns 
    }

    pid_t child = waitpid(-1, &statuspd ,0);

    int err = errno; // It is adviced to copy errno as soon as syscall ends.
    printf("waitpid returned %d, status=%d, errno=%d\n", child, statuspd, err);

    if (child < 0)
        unix_error("waitpid err\n");
    else if (WIFEXITED(statuspd) && child > 0)
        printf("child %d terminated normally with exit status: %d\n", child, WEXITSTATUS(statuspd));
    else if (WIFSIGNALED(statuspd))
        printf("child %d terminated by signal %d\n", child, WTERMSIG(statuspd));
    else
        printf("child %d terminated abnormally\n", child);

    printf("Parent terminated child with PID: %d\n", child);

    free(buf);
    free(argv);
}
