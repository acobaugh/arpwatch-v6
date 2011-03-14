#include <stdio.h>
#include <time.h>
#include <string.h>

#include "notify_syslog.h"

notify_syslog_config_t notify_syslog_config;

extern int opt_debug;

void notify_syslog(char *mac, time_t time, char *ip, int netn, int packet_type, int notify_type) {
	if (opt_debug) {
		printf("%s(): mac = %s, time = %lli, ip = %s, netn = %i, packet_type = %i, notify_type = %i\n", 
			__FUNCTION__, mac, (long long int)time, ip, netn, packet_type, notify_type
		);
	}
}

void notify_config_syslog(cfg_t *cfg) {
	strncpy(notify_syslog_config.facility, cfg_getstr(cfg, "facility"), sizeof(notify_syslog_config.facility));
	strncpy(notify_syslog_config.priority, cfg_getstr(cfg, "priority"), sizeof(notify_syslog_config.priority));
}

