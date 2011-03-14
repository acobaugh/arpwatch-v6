#ifndef CAPTURE_H
#define CAPTURE_H 1

#include <pcap.h>

/* internal packet types */
#define PACKET_TYPE_IPV6_RS 0
#define PACKET_TYPE_IPV6_RA 1
#define PACKET_TYPE_IPV6_NS 2
#define PACKET_TYPE_IPV6_NA 3
#define PACKET_TYPE_ARP_REQUEST 4
#define PACKET_TYPE_ARP_REPLY 5
#define PACKET_TYPE_RARP_REPLY 6

void capture(char *, char *filter_expr, int);

void callback(u_char *, const struct pcap_pkthdr *, const u_char *);

#endif
