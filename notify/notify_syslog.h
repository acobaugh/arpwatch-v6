#ifndef NOTIFY_SYSLOG_H
#define NOTIFY_SYSLOG_H 1

#include <confuse.h>
#include <time.h>

typedef struct notify_syslog_config {
	char facility[32];
	char priority[32];
} notify_syslog_config_t;

void notify_syslog(char *, time_t, char *, int, int, int);

void notify_config_syslog(cfg_t *);

#endif
