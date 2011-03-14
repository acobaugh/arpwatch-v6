#include <stdio.h>

#include "notify.h"
#include "utils.h"

extern int opt_debug;

lookup_table_t notify_method_table[] = {
	{"email", NOTIFY_METHOD_EMAIL},
	{"syslog", NOTIFY_METHOD_SYSLOG},
	{"", TABLE_STOP}
};

void notify(char *smac, time_t time, char *ip, int netn, int notify_type) {
	if (opt_debug) {
		printf("%s(): %s, %lli, %s, %i, %i\n", __FUNCTION__, smac, (long long int)time, ip, netn, notify_type);
	}
}
