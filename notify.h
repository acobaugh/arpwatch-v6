#ifndef NOTIFY_H
#define NOTIFY_H

#include <time.h>

#include "notify/notify_syslog.h"
#include "notify/notify_email.h"

/* notify methods */
#define NOTIFY_METHOD_SYSLOG 0
#define NOTIFY_METHOD_EMAIL 1

/* notify message types/conditions */
#define NOTIFY_NEW_MAC 0 /* never before seen mac address */
#define NOTIFY_NEW_IP 1 /* new IP for an existing mac */
#define NOTIFY_IP_CHANGED_MAC 2/* previously seen IP is now associated with a new mac */
#define NOTIFY_FLIP_FLOP 3 /* same as NOTIFY_IP_CHANGED_MAC but delta_time is < FLIPFLOP_THRESHOLD_TIME */

/* delta_time between IP flip flops in seconds */
#define FLIPFLOP_THRESHOLD_TIME 300

void notify(char *, time_t, char *, int, int);

#endif
