#ifndef SERVICE_H
#define SERVICE_H

#define MAX_CHARS 64

typedef struct Service {
    char cmdline[256];
    char fout[256];
    char ferr[256];
    char restart[64];
}service_t;

int service_load(service_t *service, const char *fname);
void service_shutdown(service_t *service);


#endif
