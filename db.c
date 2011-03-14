#include <stdio.h>
#include <string.h>

#include "db.h"
#include "config.h"
#include "utils.h"

#ifdef HAVE_MYSQL
#include "db/db_mysql.h"
#endif

extern int opt_debug;
extern network_config_t networks[];

lookup_table_t db_table[] = {
	{"mysql", DB_MYSQL},
	{"file", DB_FILE},
	{"", TABLE_STOP}
};

void db_store(char *smac, time_t time, char *ip, int netn, int packet_type) {
	int i;

	if (opt_debug) {	
		printf("db_store(): smac = %s, time = %lli, ip = %s, netn = %i, packet_type = %i\n", smac, (long long int)time, ip, netn, packet_type);
	}

	/* call db_store_foo for each database defined for this netn */
	for (i = 0; i < networks[netn].num_databases; i++) {
		switch (networks[netn].databases[i]) {
#ifdef HAVE_MYSQL
			case DB_MYSQL:
				db_store_mysql(smac, time, ip, netn, packet_type);
				break;
#endif
			default:
				if (opt_debug) {
					printf("%s(): unknown database: %i\n", __FUNCTION__, networks[netn].databases[i]);
				}
				break;
		}
	}
}


