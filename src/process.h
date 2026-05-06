#ifndef PROCESS_H
#define PROCESS_H

#include <sys/types.h>

pid_t Fork(void);
void run_service(char *cmdline);

#endif
