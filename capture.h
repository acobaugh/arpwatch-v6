#include <pcap.h>

void capture(char [], char []);

void callback(u_char *, struct pcap_pkthdr *, u_char *);
