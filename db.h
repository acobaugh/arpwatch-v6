#ifndef DB_H
#define DB_H 1

#include "db/db_file.h"

#define DB_NONE 0
#define	DB_FILE 1
#define	DB_MYSQL 2

void db_store(char *, time_t, char *, int, int);

#endif
