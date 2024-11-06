/**
 * IPK second project - variant Delta
 * @author Timotej Ponek, xponek00
 * @file IPs.cpp
 */
#include "IPs.h"
#include <inttypes.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/types.h>

//forward declaration of static class member
std::vector<IPsubnet::IP> IPsubnet::allIPs;


/**
 * @brief Extracts subnet mask from IPv4
 * function is adopted from https://codereview.stackexchange.com/a/141840
 * for closer description see IPsubnet class description
 * @param cidr ip adress without sufix
 * @param addr where will be ip adress stored as in_addr structure
 * @param mask where will be ip subnet mask stored as in_addr structure
 * @param bits sufix of ip
 */
void parse_cidr(const char *cidr, struct in_addr *addr, struct in_addr *mask, int bits) {
    inet_pton(AF_INET, cidr, addr);
    /* Shifting by 32 bits is undefined (http://stackoverflow.com/q/2648764) */
    mask->s_addr = htonl(~(bits == 32 ? 0 : ~0U >> bits));
    return;
}

/**
 * @brief Extracts subnet mask from IPv6
 * function based on answer from https://codereview.stackexchange.com/a/141840
 * for closer description see IPsubnet class description
 * @param cidr ip adress without sufix
 * @param addr where will be ip adress stored as in6_addr structure
 * @param mask where will be ip subnet mask stored as in6_addr structure
 * @param bits sufix of ip
 */
void parse_cidr6(const char *cidr, struct in6_addr *addr, struct in6_addr *mask, int bits) {
    inet_pton(AF_INET6, cidr, addr);    
    for(int i = 0; i<4; i++){
        if(bits <= 32){
            /* Shifting by 32 bits is undefined (http://stackoverflow.com/q/2648764) */
            mask->__in6_u.__u6_addr32[i] = htonl(~(bits == 32 ? 0 : ~0U >> bits));
            bits = 0U >> bits;
        }
        else{
            mask->__in6_u.__u6_addr32[i] = htonl(~(0U));
            bits = bits - 32;
        }
    }
    return;
}

/**
 * @brief Add new range ips to scan to static class member
 * @param ip adress without sufix
 * @param ipVersion IPv4 or IPv6
 * @param subnet sufix of passed ip adress
 */
IPsubnet::IPsubnet(const char* ip, int ipVersion, int subnet){    
    struct IP tmp;
    unsigned char buf[sizeof(struct in_addr)];

    /* 
        code below is from asnwer https://codereview.stackexchange.com/a/141840
        from user 200_success (https://codereview.stackexchange.com/users/9357/200-success)
        to question: https://codereview.stackexchange.com/q/141674
        and adapted for project needs
    */
    if(ipVersion == IPv4){

        static char buf[INET_ADDRSTRLEN];
        struct in_addr addr, mask, wildcard, network, broadcast, min, max;

        parse_cidr(ip, &addr, &mask, subnet);

        wildcard = mask;    wildcard.s_addr = ~wildcard.s_addr;
        network = addr;     network.s_addr &= mask.s_addr;
        broadcast = addr;   broadcast.s_addr |= wildcard.s_addr;
        min = network;      min.s_addr = htonl(ntohl(min.s_addr) + 1);
        max = broadcast;    max.s_addr = htonl(ntohl(max.s_addr) - 1);

        tmp.begin = inet_ntop(AF_INET, &min, buf, INET_ADDRSTRLEN);
        tmp.end = inet_ntop(AF_INET, &max, buf, INET_ADDRSTRLEN);

        tmp.IPRangeStart.ipv4 = min;
        tmp.IPRangeEnd.ipv4 = max;

        tmp.IPversion = IPv4;

        this->allIPs.push_back(tmp);
    }

    else if(ipVersion == IPv6){
        static char buf[INET6_ADDRSTRLEN];
        struct in6_addr addr, mask, wildcard, network, broadcast, min, max;

        parse_cidr6(ip, &addr, &mask, subnet);

        wildcard = mask;
        for(int i = 0; i<4; i++){
            wildcard.__in6_u.__u6_addr32[i] = ~wildcard.__in6_u.__u6_addr32[i];
        }
        network = addr;
        for(int i = 0; i<4; i++){
            network.__in6_u.__u6_addr32[i] &= mask.__in6_u.__u6_addr32[i];
        }
        broadcast = addr;
        for(int i = 0; i<4; i++){
            broadcast.__in6_u.__u6_addr32[i] |= wildcard.__in6_u.__u6_addr32[i];
        }

        tmp.begin = inet_ntop(AF_INET6, &network, buf, INET6_ADDRSTRLEN);
        tmp.end = inet_ntop(AF_INET6, &broadcast, buf, INET6_ADDRSTRLEN);
        
        tmp.IPRangeStart.ipv6 = min;
        tmp.IPRangeEnd.ipv6 = max;

        tmp.IPversion = IPv4;

        this->allIPs.push_back(tmp);
    }
    /**************************************************************************************/
}

/**
 * @brief default destructor
 */   
IPsubnet::~IPsubnet(){
    
}