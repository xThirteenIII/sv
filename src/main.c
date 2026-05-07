#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "process.h"
#include "service.h"

int main()
{
    service_t service;
    int ok = service_load(&service, "services/sleep.conf");
    if (ok < 0)
        return -1;
    run_service(&service);
    return 0;
}
