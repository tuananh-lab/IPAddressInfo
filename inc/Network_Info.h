#ifndef NETWORK_INFO_H
#define NETWORK_INFO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <ifaddrs.h>

char* getMACAddress(const char *interface);
char* getDefaultGateway();
char* getBroadcastAddress(const char *interface);
char* getUnicastAddress(const char *interface);
char* getInterfaceStatus(const char *interface);
char* getIPAddressInfo(struct sockaddr_in *sa, const char *subnetmask);
char* getIPv6AddressInfo(struct sockaddr_in6 *sa);
char* getSubNetmask(const char *interface);
char* calculateSubnetIP(const char *ipAddress, const char *subnetMask);
char* getIPv6Address(const char *interface);
char* getInterfaceIPAddressInfo(struct ifaddrs *ifa);
char* getAllInterfaceIPAddressInfo();

#endif /* NETWORK_INFO_H */
