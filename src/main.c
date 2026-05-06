#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "service.h"

int main()
{
    service_t service;
    service_load(&service, "services/sleep.conf");
    service_run(service.cmdline);
    return 0;
}
