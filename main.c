#include <stdio.h>
#include <getopt.h>

#include "utils.h"
#include "config.h"
#include "capture.h"

int main(int argc, char *argv[]) {
	int c;
	char *config_file;
	int debug = 0;

	/* read in command line options */
	while ((c = getopt(argc, argv, "c:d")) != -1) {
		switch(c) {
			case 'd':
				debug = 1;
				break;
			case 'c':
				config_file = optarg;
				printf("config_file = %s\n", config_file);

				break;
			default:
				usage();
				break;
		}
	}

	char dev[] = "eth0";
	char filter_expr[] = "(icmp6 or arp) and ( (net 128.118.200.0/23) or (net 2610:8:7800:14::/64) ) ";
	capture(dev, filter_expr);

	return 0;
}

