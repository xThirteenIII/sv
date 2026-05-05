#ifndef UTILITY_H
#define UTILITY_H

#include "service.h"

/* parseline reads buf and builds the argv array */
int parseline(char *buf, char **argv);
void unix_error(char *msg);
service_t *read_conf_file(const char *fname);

#endif
