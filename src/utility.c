#include "utility.h"
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include "service.h"

#define MAX_LINE 128

int parseline(char *buf, char **argv) {
    char *delim;
    int argc;

    buf[strlen(buf) - 1] = ' '; /* Replace trailing \n with space */
    while (*buf && (*buf == ' ')) /* Ignore leading spaces */
        buf++;
    /* Build argv list */
    argc = 0;
    while ((delim = strchr(buf, ' '))){ /* delim points to the first occurence of ' ' in buf string.
                                           I.e. the end of the string*/
        argv[argc++] = buf;
        *delim = '\0';
        buf = delim + 1;
        while (*buf && (*buf == ' ')) /* Ignore spaces */
            buf++;
    }
    argv[argc] = NULL;

    if (argc == 0) /* Ignore blank line */
        return 1;

    return 0;
}

service_t read_conf_file(const char *fname)
{
    service_t service;
    FILE *fp = fopen(fname, "r");
    if (!fp){
        fprintf(stderr, "Can't open file %s: %s\n", fname, strerror(errno));
        exit(EXIT_FAILURE);
    }
    /* Read line by line */
    while(!feof(fp)){
        char *line = calloc(MAX_LINE, sizeof(char));
        fgets(line, MAX_LINE, fp);
        /* Extract key:value pairs */
        entry_t entry = read_entry(line);
        free(line);
        /* Skip comments */
        if (ferror(fp) < 0)
            exit(EXIT_FAILURE);
        if (strcmp(entry.key, "cmd") == 0){
            strcpy(service.cmdline, entry.value);
            service.cmdline[strlen(entry.value)] = '\0';
        }
        else if (strcmp(entry.key, "restart") == 0){
            strcpy(service.restart, entry.value);
            service.restart[strlen(entry.value)] = '\0';
        }
        else if (strcmp(entry.key, "stdout") == 0){
            strcpy(service.fout, entry.value);
            service.fout[strlen(entry.value)] = '\0';
        }
        else if (strcmp(entry.key, "stderr") == 0){
            strcpy(service.ferr, entry.value);
            service.ferr[strlen(entry.value)] = '\0';
        }
        else
            continue;
    }
    fclose(fp);
    return service;
}

void unix_error(char *msg) {
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(EXIT_FAILURE);
}
