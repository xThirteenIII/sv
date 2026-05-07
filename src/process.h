#ifndef PROCESS_H
#define PROCESS_H

#include "service.h"
#include <sys/types.h>

pid_t Fork(void);
void run_service(service_t *service);

#endif
