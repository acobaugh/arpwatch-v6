#ifndef CONFIG_H
#define CONFIG_H 1

int config_read(char *);

typedef struct network_config {
	int foo;
	char name[64];
	char device[8];
	char filter[1024];
	int num_databases;
	char databases[2][16];
} network_config_t;

#endif
