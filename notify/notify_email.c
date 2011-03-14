#include <stdio.h>
#include <time.h>
#include <confuse.h>

#include "notify_email.h"
#include "../utils.h"

notify_email_config_t notify_email_config;

extern int opt_debug;

void notify_email(char *mac, time_t time, char *ip, int netn, int packet_type, int notify_type) {
	if (opt_debug) {
		printf("%s(): mac = %s, time = %lli, ip = %s, netn = %i, packet_type = %i, notify_type = %i\n", 
			__FUNCTION__, mac, (long long int)time, ip, netn, packet_type, notify_type
		);
	}
}

void notify_config_email(cfg_t *cfg) {
	strncpy(notify_email_config.from, cfg_getstr(cfg, "from"), sizeof(notify_email_config.from));
	strncpy(notify_email_config.to, cfg_getstr(cfg, "to"), sizeof(notify_email_config.to));
}

