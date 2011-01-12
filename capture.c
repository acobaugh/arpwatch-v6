#include <stdio.h>
#include <pcap.h>

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

#include <string.h>
#include <unistd.h> /* getopt */

#include "utils.h"

extern int opt_debug;

/* callback() gets called on every packet by pcap_loop() */
void callback(u_char *args, const struct pcap_pkthdr *hdr, const u_char *packet) {
	/* ethernet packet header */
	struct ether_header *ether_hdr_p;
	/* ethernet packet time */
	time_t time = hdr->ts.tv_sec;

	/* ipv6 */
	struct ip6_hdr *ip6_hdr_p;
	struct icmp6_hdr *icmp6_hdr_p;
	int is_icmp6 = 0;
	struct ip6_ext *ip6_ext_hdr_ptr;

	/* ipv4 and arp */
	struct ether_arp *ether_arp_p;

	char ip[NI_MAXHOST] = "";
	char hostname[NI_MAXHOST] = "";
	char smac[18];

	//fprintf(stderr, "time: %lld\n", (long long int)time); 

	/* ethernet layer */
	ether_hdr_p = (struct ether_header *)packet;
	//print_eth(*ether_hdr_p);
	strcpy(smac, ether_ntoa((struct ether_addr *)(ether_hdr_p->ether_shost)));

	/* get ether_type in host byte order */
	ether_hdr_p->ether_type = ntohs(ether_hdr_p->ether_type);

	/* IPv6 */
	if (ether_hdr_p->ether_type == ETHERTYPE_IPV6) {
		ip6_hdr_p = (struct ip6_hdr*)(packet + 14);

		/* if the next header is not an icmp6 header */
		if (ip6_hdr_p->ip6_nxt != 58)	{
			ip6_ext_hdr_ptr = (struct ip6_ext*)(packet + 14 + 40);

			/* loop through while next header is not an icmp6 header */
			while((ip6_ext_hdr_ptr->ip6e_nxt != 58) && ((u_char*)ip6_hdr_p < (packet + hdr->len)))	{
				ip6_ext_hdr_ptr = (struct ip6_ext*)((u_char*)ip6_ext_hdr_ptr + (1 + ip6_ext_hdr_ptr->ip6e_len)*8);
			}

			/* check if the loop found an icmp6 header */
			if (ip6_ext_hdr_ptr->ip6e_nxt == 58) {
				icmp6_hdr_p = (struct icmp6_hdr*)((u_char*)ip6_ext_hdr_ptr + (1 + ip6_ext_hdr_ptr->ip6e_len)*8);
				is_icmp6 = 1;
			}

		} else { /* first header is an icmp6 header */
			icmp6_hdr_p = (struct icmp6_hdr*)(packet + 14 + 40);
			is_icmp6 = 1;
		}
	
		if (is_icmp6) {
			switch (icmp6_hdr_p->icmp6_type)	{
				case ND_ROUTER_SOLICIT:
					ipv6_ntoa(ip, ip6_hdr_p->ip6_src);
					iptoname(hostname, ip);
					printf("ND_ROUTER_SOLICIT:\t %s\t%lld\t%s\t%s\n", smac, (long long int)time, ip, hostname);
					break;
				case ND_ROUTER_ADVERT:
					ipv6_ntoa(ip, ip6_hdr_p->ip6_src);
					iptoname(hostname, ip);
					printf("ND_ROUTER_ADVERT:\t %s\t%lld\t%s\t%s\n", smac, (long long int)time, ip, hostname);
					break;
				case ND_NEIGHBOR_SOLICIT:
					ipv6_ntoa(ip, ip6_hdr_p->ip6_src);
					iptoname(hostname, ip);
					printf("ND_NEIGHBOR_SOLICIT:\t %s\t%lld\t%s\t%s\n", smac, (long long int)time, ip, hostname);
					break;
				case ND_NEIGHBOR_ADVERT:
					ipv6_ntoa(ip, ip6_hdr_p->ip6_src);
					iptoname(hostname, ip);
					printf("ND_NEIGHBOR_ADVERT:\t %s\t%lld\t%s\t%s\n", smac, (long long int)time, ip, hostname);
					break;
				case ND_REDIRECT:
					printf("ND_REDIRECT\n");
					break;
				default:
					printf("ignoring packet\n");
			}
		} else {
			return;
		}
	} /* END IPv6 */


	/* ARP */
	if (ether_hdr_p->ether_type == ETHERTYPE_ARP || ether_hdr_p->ether_type == ETHERTYPE_REVARP) {
		/* check length */
		if (hdr->caplen < sizeof(*ether_hdr_p) + sizeof(*ether_arp_p)) {
			printf("Short ARP packet, ignoring\n");
			return;
		}
		
		ether_arp_p = (struct ether_arp *)(packet + 14);

		ether_arp_p->arp_op = ntohs(ether_arp_p->arp_op);
		switch (ether_arp_p->arp_op) {
			case ARPOP_REQUEST:
				ipv4_ntoa(ip, ether_arp_p->arp_spa);
				iptoname(hostname, ip);
				printf("ARP request:\t %s\t%lld\t%s\t%s\n", smac, (long long int)time, ip, hostname);
				break;
			case ARPOP_REPLY:
				ipv4_ntoa(ip, ether_arp_p->arp_spa);
				iptoname(hostname, ip);
				printf("ARP reply:  \t %s\t%lld\t%s\t%s\n", smac, (long long int)time, ip, hostname);
				break;
			case ARPOP_RREPLY:
				ipv4_ntoa(ip, ether_arp_p->arp_spa);
				iptoname(hostname, ip);
				printf("RARP reply:  \t %s\t%lld\t%s\t%s\n", smac, (long long int)time, ip, hostname);
				break;
			default:
				printf("ignoring packet\n");
		}
	} /* END ARP */
}

void capture(char *dev, char *filter_expr) {
	char errbuf[PCAP_ERRBUF_SIZE] = "";
	struct bpf_program fp;
	bpf_u_int32 mask;
	bpf_u_int32 net;

	pcap_t *handle = NULL;
	u_char *args = NULL;

	if (pcap_lookupnet(dev, &net, &mask, errbuf) == -1) {
		fprintf(stderr, "Couldn't get netmask for device %s: %s\n", dev, errbuf);
		net = 0;
		mask = 0;
	} else if (opt_debug) {
		printf("pcap_lookupnet(): net = %d mask = %d\n", net, mask);
	}

	handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
	if (handle == NULL) {
		fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
		die();
	} else if (opt_debug) {
		printf("pcap_open_live(): success\n");
	}

	if (pcap_compile(handle, &fp, filter_expr, 0, net) == -1) {
		fprintf(stderr, "Couldn't parse filter %s: %s\n", filter_expr, pcap_geterr(handle));
		die();
	} else if (opt_debug) {
		printf("pcap_compile(): success\n");
	}

	if (pcap_setfilter(handle, &fp) == -1) {
		fprintf(stderr, "Couldn't install filter %s: %s\n", filter_expr, pcap_geterr(handle));
		die();
	} else if (opt_debug) {
		printf("pcap_setfilter(): success\n");
	}

	pcap_loop(handle, 0, callback, args);

	pcap_freecode(&fp);
	pcap_close(handle);
}


