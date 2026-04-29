#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void unix_error(char *msg) {
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(0);
}

int parseline(char *buf, char **argv) {
    char *delim;
    int argc;

    buf[strlen(buf) - 1] = ' '; /* Replace trailing \n with space */
    while (*buf && (*buf == ' ')) /* Ignore leading spaces */
        buf++;
    /* Build argv list */
    argc = 0;
    while ((delim = strchr(buf, ' '))){ /* delim points to the first occurence of ' ' in buf string.
                                           I.e. the end of the string*/
        argv[argc++] = buf;
        *delim = '\0';
        buf = delim + 1;
        while (*buf && (*buf == ' ')) /* Ignore spaces */
            buf++;
    }
    argv[argc] = NULL;

    if (argc == 0) /* Ignore blank line */
        return 1;

    return 0;
}

pid_t Fork(void) {
    pid_t pid;

    if ((pid = fork()) < 0)
        unix_error("Fork error");
    return pid;
}

int main() {
    pid_t pid;
    char *buf = malloc(sizeof(char) * strlen("/usr/bin/sleep 2\n") + 1);
    int argc = 4;
    char **argv = malloc(sizeof(char*) * argc);
    strcpy(buf, "/usr/bin/sleep 2\n");
    parseline(buf, argv);

    pid = Fork();
    
    if (pid == 0) {
        printf("child executing: %s\n", argv[0]);
        execve(argv[0], argv, NULL);
        printf("terminating...\n");
        exit(0);
    }

    pid_t child = waitpid(-1, NULL ,0);
    printf("Parent terminated child with PID: %d\n", child);
    return 0;
}
