void iptoname(char *, char *);

void print_eth(struct ether_header);

void ipv4_ntoa(char *, u_int8_t []);

void ipv6_ntoa(char *, struct in6_addr);

void print_ipv6hdr(struct ip6_hdr);

void print_ipv4hdr(struct ether_arp);

