#ifndef DB_MYSQL_H
#define DB_MYSQL_H 1

/* mysql database type options */
cfg_opt_t database_mysql_opts[] = {
	CFG_STR("host", "localhost", CFGF_NONE),
	CFG_STR("db", "arpwatch", CFGF_NONE),
	CFG_STR("password", "arpwatch", CFGF_NONE),
	CFG_STR_LIST("notify_methods", NULL, CFGF_NONE),
	CFG_END()
};

typedef struct {
	char *host;
	char *db;
	char *password;
	char **notify_methods;
} db_mysql_config;


#endif
