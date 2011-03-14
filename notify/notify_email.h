#ifndef NOTIFY_EMAIL_H
#define NOTIFY_EMAIL_H 1

#include <confuse.h>

typedef struct notify_email_config {
	char from[320];
	char to[320];
} notify_email_config_t;

void notify_email(char *, time_t, char *, int, int, int);

void notify_config_email(cfg_t *);

#endif

