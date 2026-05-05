#ifndef SERVICE_H
#define SERVICE_H

#define MAX_CHARS 64

typedef struct Service {
    char *cmdline;
    char *restart;
    char *fout;
    char *ferr;
}service_t;

typedef struct Entry {
    char key[MAX_CHARS];
    char value[MAX_CHARS];
}entry_t;

entry_t read_entry(char *line);
service_t *service_init();
void service_shutdown(service_t *service);


#endif
