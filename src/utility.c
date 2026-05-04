#include "utility.h"
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

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

void read_conf_file(const char *fname)
{
    FILE *fp = fopen(fname, "r");
    if (!fp){
        fprintf(stderr, "Can't open file %s: %s\n", fname, strerror(errno));
        exit(EXIT_FAILURE);
    }
    char *c = malloc(sizeof(char));
    size_t line_len = 0;
    printf("File stream @ %p\n", fp);
    while (*c != '\n') {
        size_t bytes_read = fread(&c, sizeof(char), 1, fp);
        strn
        line_len++;
    }
    //TODO: use feof and ferror to check if EOF or error returned
    free(line);
    fclose(fp);
}

void unix_error(char *msg) {
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(EXIT_FAILURE);
}
