#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
//#include "process.h"
#include "service.h"

int main()
{
    service_t *service = service_init();
    printf("Service:%s\t%s\t%s\t%s\n", service->cmdline, service->restart, service->fout, service->ferr);
    //run_service(service->cmdline);
    service_shutdown(service);
    return 0;
}
