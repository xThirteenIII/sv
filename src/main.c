#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
//#include "process.h"
#include "utility.h"

int main()
{
    read_conf_file("services/sleep.conf");
    return 0;
}
