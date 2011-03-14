#include <stdio.h>
#include <time.h>

#include "db_file.h"
#include "../utils.h"
//#include "../notify.h"

db_file_config_t db_file_config;

extern int opt_debug;
extern lookup_table_t notify_method_table[];

void db_store_file(char *mac, time_t time, char *ip, int netn, int packet_type) {
	if (opt_debug) {
		printf("%s(): mac = %s, time = %lli, ip = %s, netn = %i, packet_type = %i\n", 
			__FUNCTION__, mac, (long long int)time, ip, netn, packet_type
		);
	}
}

void db_config_file(cfg_t *cfg) {
	int num, j;

	strncpy(db_file_config.file, cfg_getstr(cfg, "file"), sizeof(db_file_config.file));
	/* notification methods */

	db_file_config.num_notify_methods = num;
	for (j = 0; j < num; j++) {
		db_file_config.notify_methods[j] = table_lookup(notify_method_table, cfg_getnstr(cfg, "notify_method", j));
	}
}

