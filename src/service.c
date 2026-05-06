#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include "service.h"
#include "utility.h"
#include "process.h"

#define MAXARGS 32

int service_load(service_t *service, const char *path)
{
    *service = read_conf_file(path);
    return 0;
}

void service_shutdown(service_t *service)
{
}

entry_t read_entry(char *line)
{
    struct Entry entry = {
        .key = "",
        .value = "",
    };
    if (line[0] == '#' || line[0] == '\n')
        return entry;
    char delim = '=';
    bool v = false;
    size_t j = 0;
    for (size_t i = 0; i < strlen(line); (++i, ++j)){
        if (!v){
            entry.key[j] = line[i];
        }else{
            entry.value[j] = line[i];
        }
        if (line[i] == delim){
            entry.key[j]='\0';
            j=-1;
            v=true;
        }
    }
    entry.value[j]='\0';
    return entry;
}

void service_run(char *cmdline)
{
    char *buf = malloc(sizeof(char) * strlen(cmdline));
    char **argv = malloc(sizeof(char*) * MAXARGS); // allocate MAX space, to avoid bad stuff happening.
                                                   // No need to optimize for now
    strcpy(buf, cmdline);
    // cmdline read from config file has already trailing \n
    buf[strlen(buf)] = '\0'; 
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
