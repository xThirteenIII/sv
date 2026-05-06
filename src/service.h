#ifndef SERVICE_H
#define SERVICE_H

#define MAX_CHARS 64

typedef struct Service {
    char cmdline[256];
    char fout[256];
    char ferr[256];
    char restart[32];
}service_t;

typedef struct Entry {
    char key[MAX_CHARS];
    char value[MAX_CHARS];
}entry_t;

entry_t read_entry(char *line);
int service_load(service_t *service, const char *path);
void service_run(char *cmdline);
void service_shutdown(service_t *service);


#endif
