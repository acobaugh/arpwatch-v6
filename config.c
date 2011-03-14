#include <stdio.h>
#include <confuse.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "main.h"
#include "db.h"
#include "notify.h"
#include "utils.h"
#include "db/db_file.h"

#ifdef HAVE_MYSQL
#include "db/db_mysql.h"
extern db_mysql_config_t db_mysql_config;
#endif

extern int opt_debug;
extern network_config_t networks[];
extern lookup_table_t db_table[];
//extern lookup_table_t notify_method_table[];

int num_networks = 0;

int config_read(char *f) {
	char *config_default = "/etc/arpwatch-v6/arpwatch-v6.conf";
	cfg_t *cfg, *cfg_network, *cfg_databases, *cfg_notify_methods, *cfg_tmp;
	int i, j, num;

	if (f == NULL) {
		f = config_default;
	}

	/* network section options */
	cfg_opt_t network_opts[] = {
		CFG_STR("name", "default", CFGF_NONE),
		CFG_STR("device", "eth0", CFGF_NONE),
		CFG_STR("filter", "any", CFGF_NONE),
		CFG_STR_LIST("databases", "{default}", CFGF_NONE),
		CFG_END()
	};

#ifdef HAVE_MYSQL
	/* mysql database type options */
	cfg_opt_t database_mysql_opts[] = {
		CFG_STR("host", "localhost", CFGF_NONE),
		CFG_STR("db", "arpwatch", CFGF_NONE),
		CFG_STR("password", "arpwatch", CFGF_NONE),
		CFG_STR("user", "arpwatch", CFGF_NONE),
		CFG_STR_LIST("notify_methods", NULL, CFGF_NONE),
		CFG_END()
	};
#endif

	/* file database type options */
	cfg_opt_t database_file_opts[] = {
		CFG_STR("file", "/var/arpwatch-v6/arp.dat", CFGF_NONE),
		CFG_END()
	};
	
	/* database and notification section 'type' */
	cfg_opt_t database_opts[] = { 
		CFG_SEC("file", database_file_opts, CFGF_NONE),
#ifdef HAVE_MYSQL
		CFG_SEC("mysql", database_mysql_opts, CFGF_NONE),
#endif
		CFG_END()
	};


	/* email notification method options */
	cfg_opt_t notify_method_email_opts[] = {
		CFG_STR("from", NULL, CFGF_NONE),
		CFG_STR("to", NULL, CFGF_NONE),
		CFG_END()
	};

	/* syslog notification method options */
	cfg_opt_t notify_method_syslog_opts[] = {
		CFG_STR("facility", "daemon", CFGF_NONE),
		CFG_STR("priority", "info", CFGF_NONE),
		CFG_END()
	};

	/* notify methods */
	cfg_opt_t notify_method_opts[] = {
		CFG_SEC("email", notify_method_email_opts, CFGF_NONE),
		CFG_SEC("syslog", notify_method_syslog_opts, CFGF_NONE),
		CFG_END()
	};

	/* top-most config file options/sections */
	cfg_opt_t opts[] = {
		CFG_SEC("network", network_opts, CFGF_MULTI),
		CFG_SEC("databases", database_opts, CFGF_NONE),
		CFG_SEC("notify_methods", notify_method_opts, CFGF_NONE),
		CFG_END()
	};

	/* first try the config file passed into read_config() */
	cfg = cfg_init(opts, CFGF_NONE);

	switch(cfg_parse(cfg, f)) {
		case CFG_FILE_ERROR:
			fprintf(stderr, "Could not open config file, %s\n", f);
			cfg_free(cfg);
			return 0;
			break;
		case CFG_PARSE_ERROR:
			fprintf(stderr, "Parse error reading config file, %s\n", f);
			cfg_free(cfg);
			return 0;
			break;
		case CFG_SUCCESS:
			if (opt_debug) {
				printf("Read config file, %s\n", f);
			}
			break;
		default:
			fprintf(stderr, "Unknown error reading config file, %s\n", f);
			cfg_free(cfg);
			return 0;
			break;
	}

	num_networks = cfg_size(cfg, "network");
	/* networks */
	for (i = 0; i < num_networks; i++) {
		cfg_network = cfg_getnsec(cfg, "network", i);
		strncpy(networks[i].name, cfg_getstr(cfg_network, "name"), sizeof(networks[i].name));
		strncpy(networks[i].device, cfg_getstr(cfg_network, "device"), sizeof(networks[i].name));
		strncpy(networks[i].filter, cfg_getstr(cfg_network, "filter"), sizeof(networks[i].name));
		num = cfg_size(cfg_network, "databases");
		networks[i].num_databases = num;
		for (j = 0; j < num; j++) {
			networks[i].databases[j] = table_lookup(db_table, cfg_getnstr(cfg_network, "databases", j));
		}
	}
	/* store database settings */
	cfg_databases = cfg_getsec(cfg, "databases");
#ifdef HAVE_MYSQL
	/* mysql */
	cfg_tmp = cfg_getsec(cfg_databases, "mysql");
	if (cfg_tmp != 0) {
		db_config_mysql(cfg_tmp);
	}
#endif
	/* file */
	cfg_tmp = cfg_getsec(cfg_databases, "file");
	if (cfg_tmp != 0) {
		db_config_file(cfg_tmp);
	}

	/* store notify_method settings */
	cfg_notify_methods = cfg_getsec(cfg, "notify_methods");
	/* syslog */
	cfg_tmp = cfg_getsec(cfg_notify_methods, "syslog");
	if (cfg_tmp != 0) {
		notify_config_syslog(cfg_tmp);
	}
	/* email */
	cfg_tmp = cfg_getsec(cfg_notify_methods, "email");
	if (cfg_tmp != 0) {
		notify_config_email(cfg_tmp);
	}

	/* be free my children! */
	cfg_free(cfg);
	/*
	cfg_free(cfg_network);
	cfg_free(cfg_databases);
	cfg_free(cfg_notify_methods);
	cfg_free(cfg_tmp);*/

	return 1;
}
