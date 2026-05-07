#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "service.h"
#include "utility.h"

#define MAXARGS 64

pid_t Fork(void)
{
    pid_t pid;

    if ((pid = fork()) < 0)
        unix_error("Fork error");
    return pid;
}

static int ensure_parent_dir(const char *path)
{
    char buf[MAXARGS];
    char dir[MAXARGS];
    snprintf(buf, sizeof(buf), "%s", path);
    size_t start = 0;
    for (size_t i = 0; i < strlen(buf); ++i){
        if (buf[i] == '/'){
            dir[start] = '\0';
            if (!mkdir(dir, 0777))
                printf("Created dir: %s\n", dir);
            start = 0;
            continue;
        }
        dir[start++] = buf[i];
    }
    return 0;
}

static void redirect_stdout(char *fname){
        ensure_parent_dir(fname);
        /*
         * O_APPEND may lead to corrupted files on NFS
         * filesystems if more than one process appends data
         * to a file at once.  This is because NFS does not
         * support appending to a file, so the client kernel
         * has to simulate it, which can't be done without a
         * race condition.
         */
        int fdout = open(fname, O_CREAT | O_RDWR | O_APPEND, 0777);
        if (fdout < 0){
            int err = errno; // It is adviced to copy errno as soon as syscall ends.
            printf("dup2 failed with errno: %s", strerror(err));
            exit(EXIT_FAILURE);
        }
        if (dup2(fdout, STDOUT_FILENO) < 0) {
            int err = errno; // It is adviced to copy errno as soon as syscall ends.
            printf("dup2 failed with errno: %s", strerror(err));
            exit(EXIT_FAILURE);
        }
}

static void redirect_stderr(char *fname){
        ensure_parent_dir(fname);
        int fdout = open(fname, O_CREAT | O_RDWR | O_APPEND, 0777);
        if (fdout < 0){
            int err = errno; // It is adviced to copy errno as soon as syscall ends.
            printf("dup2 failed with errno: %s", strerror(err));
            exit(EXIT_FAILURE);
        }
        if (dup2(fdout, STDERR_FILENO) < 0) {
            int err = errno; // It is adviced to copy errno as soon as syscall ends.
            printf("dup2 failed with errno: %s", strerror(err));
            exit(EXIT_FAILURE);
        }
}
void run_service(service_t *service)
{
    char *buf = malloc(sizeof(char) * strlen(service->cmdline) + 1);
    char **argv = malloc(sizeof(char*) * MAXARGS); // allocate MAX space, to avoid bad stuff happening.
                                                   // No need to optimize for now
    strcpy(buf, service->cmdline);
    // cmdline read from config file has already trailing \n
    parseline(buf, argv);

    pid_t pid;
    int statuspd;

    // Ctrl+C kills both parent and child, thus I can't see if the child is being killed or not.
    // For testing, let's ignore the SIGINT in the parent, and put DEFAULT behaviour in the child.
    // This works.
    signal(SIGINT, SIG_IGN);
    pid = Fork();

    if (pid == 0) {
        ensure_parent_dir(service->fout);
        redirect_stdout(service->fout);
        redirect_stderr(service->ferr);
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

    if (errno != 0)
        printf("errno: %s\n", strerror(errno));

    free(buf);
    free(argv);
}

