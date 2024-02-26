#include "Network_Info.h"



// Function to retrieve the MAC address of a network interface
char* getMACAddress(const char *interface) {
    int sockfd;
    struct ifreq ifr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    strncpy(ifr.ifr_name, interface, IFNAMSIZ - 1);
    if (ioctl(sockfd, SIOCGIFHWADDR, &ifr) < 0) {
        perror("ioctl - SIOCGIFHWADDR");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    close(sockfd);

    unsigned char *mac = (unsigned char *)ifr.ifr_hwaddr.sa_data;
    char *mac_address = malloc(18);  // 6 pairs of hexadecimal digits + 5 colons + null terminator
    sprintf(mac_address, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return mac_address;
}

// Function to retrieve the default gateway address
char* getDefaultGateway() {
    FILE *fp;
    char line[256], *iface, *dest, *gw, *flags;

    // Open the route file for reading
    fp = fopen("/proc/net/route", "r");
    if (fp == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    // Read the file line by line and save to line value
    while (fgets(line, sizeof(line), fp)) {
        iface = strtok(line, "\t");
        dest = strtok(NULL, "\t");
        gw = strtok(NULL, "\t");//
        flags = strtok(NULL, "\t");

        // Check if the line represents the default route
        if (iface != NULL && dest != NULL && gw != NULL && flags != NULL && strcmp(dest, "00000000") == 0 && strcmp(flags, "0003") == 0) {
            // Close the file and return the default gateway address
            //Convert gateway address from hexa to string
            fclose(fp);
            struct in_addr addr;
            addr.s_addr = strtoul(gw, NULL, 16);
            char *gateway_address = strdup(inet_ntoa(addr));
            return gateway_address;
        }
    }
    fclose(fp);
    return NULL;
}

// Function to retrieve the broadcast address of a network interface
char* getBroadcastAddress(const char *interface) {
    int sockfd;
    struct ifreq ifr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    strncpy(ifr.ifr_name, interface, IFNAMSIZ - 1);
    if (ioctl(sockfd, SIOCGIFBRDADDR, &ifr) < 0) {
        perror("ioctl - SIOCGIFBRDADDR");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    close(sockfd);

    struct sockaddr_in *addr = (struct sockaddr_in *)&ifr.ifr_broadaddr;
    char* broadcast_address = strdup(inet_ntoa(addr->sin_addr));
    return broadcast_address;
}


// Function to retrieve the unicast address of a network interface
char* getUnicastAddress(const char *interface) {
    int sockfd;
    struct ifreq ifr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    strncpy(ifr.ifr_name, interface, IFNAMSIZ - 1);
    if (ioctl(sockfd, SIOCGIFDSTADDR, &ifr) < 0) {
        perror("ioctl - SIOCGIFDSTADDR");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    close(sockfd);

    struct sockaddr_in *addr = (struct sockaddr_in *)&ifr.ifr_dstaddr;
    char* unicast_address = strdup(inet_ntoa(addr->sin_addr));
    return unicast_address;
}
char* getInterfaceStatus(const char *interface) {
    int sockfd;
    struct ifreq ifr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    strncpy(ifr.ifr_name, interface, IFNAMSIZ - 1);
    if (ioctl(sockfd, SIOCGIFFLAGS, &ifr) < 0) {
        perror("ioctl - SIOCGIFFLAGS");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    close(sockfd);

    char* status = (ifr.ifr_flags & IFF_UP) ? "UP" : "DOWN";
    return strdup(status);
}
// Function to print IP address range for an IPv4 address
char* getIPAddressInfo(struct sockaddr_in *sa,const char *subnetmask) {
    char *result = malloc(256); 
    result[0] ='\0'; // Initialize the string
    char ip_address[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(sa->sin_addr), ip_address, INET_ADDRSTRLEN);

    struct in_addr addr, mask;
    inet_aton(ip_address, &addr);
    inet_aton(subnetmask, &mask);

    struct in_addr network;
    network.s_addr = addr.s_addr & mask.s_addr;

    struct in_addr broadcast;
    broadcast.s_addr = network.s_addr | ~mask.s_addr;

    sprintf(result + strlen(result), "IP addresses range: %s - ", inet_ntoa(network));
    // Set host bits of network address to 1 to obtain broadcast address
    broadcast.s_addr |= ~mask.s_addr;
    sprintf(result + strlen(result), "%s\n", inet_ntoa(broadcast));
    sprintf(result + strlen(result), "Subnetmask: %s\n", subnetmask);

    return result;
}
// Function to print subnet mask and IP address range for an IPv6 address
char* getIPv6AddressInfo(struct sockaddr_in6 *sa) {
    char *result = malloc(256); // Allocate enough space
    char ip6_address[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET6, &(sa->sin6_addr), ip6_address, INET6_ADDRSTRLEN);
    sprintf(result, "IPv6 Address: %s\n", ip6_address);
    return result;
}
// Function to retrieve the network's netmask (subnet mask) of a network interface
char* getSubNetmask(const char *interface) {
    int sockfd;
    struct ifreq ifr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    strncpy(ifr.ifr_name, interface, IFNAMSIZ - 1);
    if (ioctl(sockfd, SIOCGIFNETMASK, &ifr) < 0) {
        perror("ioctl - SIOCGIFNETMASK");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    close(sockfd);

    struct sockaddr_in *addr = (struct sockaddr_in *)&ifr.ifr_netmask;
    char* subnetmask = strdup(inet_ntoa(addr->sin_addr));
    return subnetmask;
}
// Function to calculate subnet IP address
char* calculateSubnetIP(const char *ipAddress, const char *subnetMask) {
    struct in_addr addr, mask, subnet;
    inet_aton(ipAddress, &addr);
    inet_aton(subnetMask, &mask);

    subnet.s_addr = addr.s_addr & mask.s_addr;

    return strdup(inet_ntoa(subnet));
}
// Function to retrieve the IPv6 address of a network interface
char* getIPv6Address(const char *interface) {
    int sockfd;
    struct ifaddrs *ifaddr, *ifa;
    struct sockaddr_in6 *sa6;

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    char *ipv6_address = NULL;

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET6) {
            if (strcmp(ifa->ifa_name, interface) == 0) {
                sa6 = (struct sockaddr_in6 *)ifa->ifa_addr;
                ipv6_address = malloc(INET6_ADDRSTRLEN);
                inet_ntop(AF_INET6, &(sa6->sin6_addr), ipv6_address, INET6_ADDRSTRLEN);
                break;
            }
        }
    }

    freeifaddrs(ifaddr);
    return ipv6_address;
}

// Function to retrieve network interface information and return IP address details
char* getInterfaceIPAddressInfo(struct ifaddrs *ifa) {
    if (ifa->ifa_addr == NULL || (ifa->ifa_flags & IFF_UP) == 0) {
        return NULL;
    }

    if (ifa->ifa_addr->sa_family == AF_INET) {
        struct sockaddr_in *sa = (struct sockaddr_in *)ifa->ifa_addr;
        char ip_address[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(sa->sin_addr), ip_address, INET_ADDRSTRLEN);


        char *subnetmask = getSubNetmask(ifa->ifa_name);
        char *result = malloc(512); // Allocate enough space
        sprintf(result, "Interface: %s\n", ifa->ifa_name);
        sprintf(result + strlen(result), "IPv4 Address: %s\n", ip_address);
        sprintf(result + strlen(result), "SubnetIP: %s\n", calculateSubnetIP(ip_address,subnetmask));
        sprintf(result + strlen(result), "Network Status: %s\n", getInterfaceStatus(ifa->ifa_name));

        strcat(result, getIPAddressInfo(sa,subnetmask));

        // Get MAC address, default gateway, broadcast address, and unicast address
        char *mac_address = getMACAddress(ifa->ifa_name);
        char *gateway_address = getDefaultGateway();
        char *broadcast_address = getBroadcastAddress(ifa->ifa_name);
        char *unicast_address = getUnicastAddress(ifa->ifa_name);
        char *ipv6_address = getIPv6Address(ifa->ifa_name);
         

        // Append the additional information to the result
        sprintf(result + strlen(result), "MAC Address: %s\n", mac_address);
        sprintf(result + strlen(result), "Default Gateway: %s\n", gateway_address);
        sprintf(result + strlen(result), "Broadcast Address: %s\n", broadcast_address);
        sprintf(result + strlen(result), "Unicast Address: %s\n", unicast_address);
        if (ipv6_address) {
            sprintf(result + strlen(result), "IPv6 Address:%s\n", ipv6_address);
            free(ipv6_address);
    }

        // Free the allocated memory
        free(subnetmask);
        free(mac_address);
        free(gateway_address);
        free(broadcast_address);
        free(unicast_address);

        strcat(result, "-----------------\n");

        return result;
    }

    return NULL;
}

// Function to return information about all active network interfaces
char* getAllInterfaceIPAddressInfo() {
    struct ifaddrs *ifaddr, *ifa;

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }
// Calculate the size to allocate to the resulting string
     size_t totalLength = 0;
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        char *info = getInterfaceIPAddressInfo(ifa);
        if (info != NULL) {
            totalLength += strlen(info);
            free(info);
        }
    }
// Allocate memory for the result string
     char *result = malloc(totalLength + 1);
    if (result == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    result[0] = '\0';
// Append information from each network interface to the result string
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        char *info = getInterfaceIPAddressInfo(ifa);
        if (info != NULL) {
            strcat(result, info);
            free(info);
        }
    }

    freeifaddrs(ifaddr);
    return result;
}
