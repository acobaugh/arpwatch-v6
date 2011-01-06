#include <stdio.h>
#include <stdlib.h>

#include <netinet/ether.h>
#include <net/ethernet.h>

#include <netinet/ip6.h>
#include <netinet/icmp6.h>

#include <netinet/ip.h>
#include <net/if_arp.h>

#ifndef ETHERTYPE_IPV6
# define ETHERTYPE_IPV6 0x86dd
#endif

#include <sys/socket.h>
#include <netdb.h>

#include "utils.h"

void usage() {
	printf("Usage: arpwatch-v6 [-d] [-c config_file]\n\n");
	die();
}

void die() {
	printf("exiting\n\n");
	exit(EXIT_FAILURE);
}

/* print layer 1 info */
void print_eth(struct ether_header eth_hdr_ptr) {
	fprintf(stderr,"Source mac address: %s \n",ether_ntoa((struct ether_addr*) (eth_hdr_ptr.ether_shost)));
	fprintf(stderr,"Destination mac address: %s \n",ether_ntoa((struct ether_addr*)(eth_hdr_ptr.ether_dhost)));
}

/* print the ipv6 addr in a readable form */
void ipv4_ntoa(char *buffer, u_int8_t addr[]) {
	sprintf (buffer, "%d.%d.%d.%d", addr[0], addr[1], addr[2], addr[3]);
}

/* print the ipv6 addr in a readable form */
void ipv6_ntoa(char *ip, struct in6_addr addr) {
	sprintf (ip, "%x:%x:%x:%x:%x:%x:%x:%x",
		((addr).s6_addr[0] << 8) | (addr).s6_addr[1],
		((addr).s6_addr[2] << 8) | (addr).s6_addr[3],
		((addr).s6_addr[4] << 8) | (addr).s6_addr[5],
		((addr).s6_addr[6] << 8) | (addr).s6_addr[7],
		((addr).s6_addr[8] << 8) | (addr).s6_addr[9],
		((addr).s6_addr[10] << 8) | (addr).s6_addr[11],
		((addr).s6_addr[12] << 8) | (addr).s6_addr[13],
		((addr).s6_addr[14] << 8) | (addr).s6_addr[15]);
}

/* print the ipv6 src/dst address */
void print_ipv6hdr(struct ip6_hdr header) {
	char ip[NI_MAXHOST];
	char hostname[NI_MAXHOST];

	ipv6_ntoa(ip, header.ip6_src);
	iptoname(hostname, ip);
	fprintf(stderr,"Source ipv6: %s\n", hostname);
	
	ipv6_ntoa(ip, header.ip6_dst);
	iptoname(hostname, ip);
	fprintf(stderr,"Destination ipv6: %s\n", ip);
	//fprintf(stderr,"Next header type: %d\n", header.ip6_nxt);
}

/* print the ipv4 src/dst address */
void print_ipv4hdr(struct ether_arp header) {
	char ip[NI_MAXHOST];
	char hostname[NI_MAXHOST];

	ipv4_ntoa(ip, header.arp_spa);
	iptoname(hostname, ip);
	fprintf(stderr, "Source ipv4 address: %s\n", hostname);
	
	ipv4_ntoa(ip, header.arp_tpa);
	iptoname(hostname, ip);
	fprintf(stderr, "Destination ipv4 address: %s\n", hostname);
}


/* converts ipv4 or ipv6 address to a hostname */
void iptoname(char *hostname, char *ip) {
	int ret = 0;
	struct addrinfo *ai;

	ret = getaddrinfo(ip, NULL, NULL, &ai);
	if (ret != 0) {
		hostname = ip;
		return;
	}

	ret = getnameinfo(ai->ai_addr, ai->ai_addrlen, hostname, NI_MAXHOST, NULL, 0, NI_NAMEREQD);

	freeaddrinfo(ai);
}

