#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <unistd.h>
#include "service.h"

#define MAX_LINE 128

typedef struct Entry {
    char key[MAX_CHARS];
    char value[MAX_CHARS];
}entry_t;

static entry_t read_entry(char *line)
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

int service_load(service_t *service, const char *path)
{
    FILE *fp = fopen(path, "r");
    if (!fp){
        fprintf(stderr, "Can't open file %s: %s\n", path, strerror(errno));
        exit(EXIT_FAILURE);
    }
    /* Read line by line */
    char line[MAX_LINE];

    // checking if fgets returns NULL is better than !feof(fp)
    // because fgets stops exactly when it encounters EOF (or error), while feof returns after having read EOF
    // and does one more iteration
    while(fgets(line, MAX_LINE, fp) != NULL){
        /* Extract key:value pairs */
        entry_t entry = read_entry(line);
        /* Skip comments */
        if (ferror(fp) < 0)
            return -1;
        if (strcmp(entry.key, "cmd") == 0){
            // strcpy copies null terminator, but doesn't check for buffer overflow
            // We use strnprintf that handles it automatically
            snprintf(service->cmdline, sizeof(service->cmdline), "%s", entry.value);
            // We don't check for traling '\n' because parseline function already takes care of that.
        }
        else if (strcmp(entry.key, "restart") == 0){
            snprintf(service->restart, sizeof(service->restart), "%s", entry.value);
            char *delim;
            if ((delim = strchr(service->restart, '\n')))
                *delim = '\0';
        }
        else if (strcmp(entry.key, "stdout") == 0){
            snprintf(service->fout, sizeof(service->fout), "%s", entry.value);
            char *delim;
            if ((delim = strchr(service->fout, '\n')))
                *delim = '\0';
        }
        else if (strcmp(entry.key, "stderr") == 0){
            snprintf(service->ferr, sizeof(service->ferr), "%s", entry.value);
            char *delim;
            if ((delim = strchr(service->ferr, '\n')))
                *delim = '\0';
        }
        else
            continue;
    }
    fclose(fp);
    return 0;
}

void service_shutdown(service_t *service)
{
}
