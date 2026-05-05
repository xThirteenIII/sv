#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "service.h"
#include "utility.h"

service_t *service_init()
{
    service_t *service = read_conf_file("services/sleep.conf");
    return service;
}

void service_shutdown(service_t *service)
{
    free(service);
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
