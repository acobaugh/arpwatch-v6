#include <stdio.h>
#include <confuse.h>
#include "config.h"

int read_config(char *f) {
	char *config_default = "/etc/arpwatch-v6/arpwatch-v6.conf";
	cfg_t *cfg;
	int error;

	if (f == NULL) {
		f = config_default;
	}

	/* network section options */
	cfg_opt_t network_opts[] = {
		CFG_STR("name", "default", CFGF_NONE),
		CFG_STR("device", "eth0", CFGF_NONE),
		CFG_STR("filter", "any", CFGF_NONE),
		CFG_STR_LIST("databases", "{default}", CFGF_NONE)
	};


	/* mysql database type options */
	cfg_opt_t database_mysql_opts[] = {
		CFG_STR("host", "localhost", CFGF_NONE),
		CFG_STR("db", "arpwatch", CFGF_NONE),
		CFG_STR("password", "arpwatch", CFGF_NONE),
		CFG_STR_LIST("notify_methods", NULL, CFGF_NONE)
	};

	/* file database type options */
	cfg_opt_t database_file_opts[] = {
		CFG_STR("file", "/var/arpwatch-v6/arp.dat", CFGF_NONE)
	};
	
	/* database and notification section 'type' */
	cfg_opt_t database_opts[] = { 
		CFG_SEC("mysql", database_mysql_opts, CFGF_TITLE | CFGF_MULTI),
		CFG_SEC("file", database_file_opts, CFGF_TITLE | CFGF_MULTI)
	};

	/* email notification method options */
	cfg_opt_t notify_method_email_opts[] = {
		CFG_STR("from", NULL, CFGF_NONE),
		CFG_STR("to", NULL, CFGF_NONE)
	};

	/* syslog notification method options */
	cfg_opt_t notify_method_syslog_opts[] = {
		CFG_STR("facility", "daemon", CFGF_NONE),
		CFG_STR("priority", "info", CFGF_NONE)
	};

	cfg_opt_t notify_method_opts[] = {
		CFG_SEC("email", notify_method_email_opts, CFGF_TITLE | CFGF_MULTI),
		CFG_SEC("syslog", notify_method_syslog_opts, CFGF_TITLE | CFGF_MULTI)
	};

	/* top-most config file options/sections */
	cfg_opt_t opts[] = {
		CFG_SEC("network", network_opts, CFGF_MULTI),
		CFG_SEC("databases", database_opts, CFGF_NONE),
		CFG_SEC("notify_methods", notify_method_opts, CFGF_NONE)
	};

	/* first try the config file passed into read_config() */
	cfg = cfg_init(opts, CFGF_NONE);
	
	switch(cfg_parse(cfg, f)) {
		case CFG_FILE_ERROR:
			printf(stderr, "Could not open config file, %s\n", f);
			cfg_free(cfg);
			return 0;
			break;
		case CFG_PARSE_ERROR:
			printf(stderr, "Parse error reading config file, %s\n", f);
			cfg_free(cfg);
			return 0;
			break;
		case CFG_SUCCESS:
			printf("Read config file, %s\n", f);
			return 1;
			break;
		default:
			printf(stderr, "Unknown error reading config file, %s\n", f);
			cfg_free(cfg);
			return 0;
			break;
	}

	cfg_free(cfg);

	return 0;
}
