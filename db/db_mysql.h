#ifndef DB_MYSQL_H
#define DB_MYSQL_H 1

#include <time.h>
#include <confuse.h>

typedef struct db_mysql_config {
	char host[128];
	char db[64];
	char user[64];
	char password[128];
	int num_notify_methods;
	int notify_methods[2];
} db_mysql_config_t;

void db_store_mysql(char *, time_t, char *, int, int);

void db_config_mysql(cfg_t *);

void db_mysql_error(int err);

#endif
