#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

#include <linux/if.h>
#include <linux/if_tun.h>

#include <netinet/in.h>
#include <netpacket/packet.h>
#include <arpa/inet.h>

#include "tap.h"

int tun_alloc(char *dev)
{
	struct ifreq ifr;
	int fd, err;

	if( (fd = open("/dev/net/tun", O_RDWR)) < 0 ) {
		perror("Opening /dev/net/tun");
		return fd;
	}

	memset(&ifr, 0, sizeof(ifr));

	ifr.ifr_flags = IFF_TAP | IFF_NO_PI;
	strncpy(ifr.ifr_name, dev, IFNAMSIZ);

	if( (err = ioctl(fd, TUNSETIFF, (void *)&ifr)) < 0 ) {
		perror("ioctl(TUNSETIFF)");
		close(fd);
		return err;
	}

	return fd;
}

void tap_config(const char * ifname, const char * ipaddr, const char *netmask)
{
	struct sockaddr_in* sin;
	struct ifreq req;
	int fd;

	fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);

	/* setup ipaddr */
	memset(&req, 0, sizeof(req));
	strcpy(req.ifr_name, ifname);

	sin = (struct sockaddr_in*)&req.ifr_addr;
	sin->sin_family = AF_INET;
	inet_aton(ipaddr, &sin->sin_addr);
	ioctl(fd, SIOCSIFADDR, &req);
	
	/* setup netmask */
	memset(&req, 0, sizeof(req));
	strcpy(req.ifr_name, ifname);
	sin = (struct sockaddr_in*)&req.ifr_addr;
	sin->sin_family = AF_INET;
	inet_aton(netmask, &sin->sin_addr);
	ioctl(fd, SIOCSIFNETMASK, &req);

	/* set device up */
	memset(&req, 0, sizeof(req));
	strcpy(req.ifr_name, ifname);

	ioctl(fd, SIOCGIFFLAGS, &req);
	req.ifr_flags |= IFF_UP;
	ioctl(fd, SIOCSIFFLAGS, &req);
}
