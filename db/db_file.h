#ifndef DB_FILE_H
#define DB_FILE_H 1

#include <time.h>
#include <confuse.h>

typedef struct db_file_config {
	char file[256];
	int num_notify_methods;
	int notify_methods[2];
} db_file_config_t;

void db_store_file(char *, time_t, char *, int, int);

void db_config_file(cfg_t *);

#endif
