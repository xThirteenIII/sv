#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "process.h"

int main() {
    run_service("/usr/bin/sleep 2");
    return 0;
}
