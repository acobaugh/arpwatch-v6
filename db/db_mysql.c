#include <stdio.h>
#include <time.h>
#include <confuse.h>

#include <my_global.h>
#include <mysql.h>
#include <mysql/errmsg.h>

#include "db_mysql.h"
#include "../notify.h"
#include "../utils.h"

db_mysql_config_t db_mysql_config;

extern int opt_debug;
extern lookup_table_t notify_method_table[];

MYSQL *mysql_conn;

void db_store_mysql(char *mac, time_t time, char *ip, int netn, int packet_type) {
	if (opt_debug) {
		printf("%s(): mac = %s, time = %lli, ip = %s, netn = %i, packet_type = %i\n", 
			__FUNCTION__, mac, (long long int)time, ip, netn, packet_type
		);
	}

	int err;
	/* insert query: 57 + values */
	char query[128];
	MYSQL_ROW row;
	MYSQL_RES *result;
	unsigned int num_rows;

	/* find existing IPs for this mac */
	sprintf(query, "SELECT * from arpwatch WHERE mac = '%s'", mac);
	err = mysql_query(mysql_conn, query);
	if (err) {
		db_mysql_error(err);
	} else {
		result = mysql_store_result(mysql_conn);
		num_rows = mysql_num_rows(result);
		printf("num_rows = %i\n", num_rows);
		if (num_rows == 0) {
			printf("New Station: %s %s %lli\n", mac, ip, (long long int)time);
			/* insert new mac address */
			memset(query, (int)"\0", sizeof(query));
			sprintf(query, "INSERT INTO arpwatch(mac, ip, timestamp) VALUES('%s', '%s', %lli)", mac, ip, (long long int)time);
			err = mysql_query(mysql_conn, query);
			db_mysql_error(err);
		}
	}
}

void db_mysql_error(int err) {
	if (opt_debug) {
		if (err != 0) {
			switch(err) {
				case CR_COMMANDS_OUT_OF_SYNC:
					printf("%s(): Commands out of sync.\n", __FUNCTION__);
					break;
				case CR_SERVER_GONE_ERROR:
					printf("%s(): Server has gone away.\n", __FUNCTION__);
					break;
				case CR_SERVER_LOST:
					printf("%s(): Connection to server lost during query.\n", __FUNCTION__);
					break;
				case CR_UNKNOWN_ERROR:
					printf("%s(): Unknown error.\n", __FUNCTION__);
					break;
			}
			printf("%s(): %s\n", __FUNCTION__, mysql_error(mysql_conn));
		}
	}
}


void db_config_mysql(cfg_t * cfg) {
	int num, j;

	if (opt_debug) {
		printf("%s(): Client version = %s\n", __FUNCTION__, mysql_get_client_info());
	}

	strncpy(db_mysql_config.host, cfg_getstr(cfg, "host"), sizeof(db_mysql_config.host));
	strncpy(db_mysql_config.db, cfg_getstr(cfg, "db"), sizeof(db_mysql_config.db));
	strncpy(db_mysql_config.password, cfg_getstr(cfg, "password"), sizeof(db_mysql_config.password));
	strncpy(db_mysql_config.user, cfg_getstr(cfg, "user"), sizeof(db_mysql_config.user));
	/* notification methods */

	db_mysql_config.num_notify_methods = num;
	for (j = 0; j < num; j++) {
		db_mysql_config.notify_methods[j] = table_lookup(notify_method_table, cfg_getnstr(cfg, "notify_method", j));
	}

	mysql_conn = mysql_init(NULL);

	if (mysql_real_connect(
			mysql_conn, 
			db_mysql_config.host, 
			db_mysql_config.user,
			db_mysql_config.password,
			db_mysql_config.db,
			0, NULL, 0) == NULL) {
		printf("Error: %u: %s\n", mysql_errno(mysql_conn), mysql_error(mysql_conn));
	} else if (opt_debug) {
		printf("%s(): mysql_real_connect(): success\n", __FUNCTION__);
	}
}

