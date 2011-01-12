#ifndef UTILS_H
#define UTILS_H 1

#include <netinet/ether.h>
#include <net/ethernet.h>

#include <netinet/ip6.h>
#include <netinet/icmp6.h>

#include <netinet/ip.h>
#include <net/if_arp.h>

void usage();

void die();

void iptoname(char *, char *);

void print_eth(struct ether_header);

void ipv4_ntoa(char *, u_int8_t []);

void ipv6_ntoa(char *, struct in6_addr);

void print_ipv6hdr(struct ip6_hdr);

void print_ipv4hdr(struct ether_arp);

#endif
