#ifndef _TAP_H_
#define _TAP_H_

int tun_alloc(char *dev);
void tap_config(const char * ifname, const char * ipaddr, const char *netmask);

#endif
