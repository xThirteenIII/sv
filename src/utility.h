#ifndef UTILITY_H
#define UTILITY_H

/* parseline reads buf and builds the argv array */
int parseline(char *buf, char **argv);
void unix_error(char *msg);
void read_conf_file(const char *fname);

#endif
