/**
 * IPK second project - variant Delta
 * @author Timotej Ponek, xponek00
 * @file hosts.cpp
 */
#include <arpa/inet.h>
#include <math.h> 
#include "hosts.h"
#include "IPs.h"

/**
 * @brief Prints number of hosts for given subnets (both IPv4 and IPv6)
 * @param IPs vector with all subnets given as launch arguments
 * @return bool true if nmbs of hosts were succesfully extracted from subnets, false on failure
 */
bool PrintNmbOfHosts(std::vector<std::string> IPs)
{
    printf("Scanning ranges\n");

    for(std::string IP : IPs){

        unsigned char buf[sizeof(struct in_addr)];
        unsigned char buf6[sizeof(struct in6_addr)];
        
        // works with both IPv4 and IPv6, see https://man7.org/linux/man-pages/man3/inet_pton.3.html
        if(inet_pton(AF_INET, IP.substr(0, IP.find('/')).c_str(), buf))
        {
            int index = IP.find('/') + 1;
            if(index != 0){
                const char *hosts = IP.substr(index, IP.npos).c_str();

                int nmbHosts = strtol(hosts, NULL, 10);
                if(nmbHosts){
                    
                    unsigned long actualHosts =  pow(2, 32 - nmbHosts) - 2;
                    printf("%s (%lu hosts)\n",IP.c_str(), actualHosts);
                    
                    // create host range entry
                    IPsubnet(IP.substr(0, IP.find('/')).c_str(), IPv4, nmbHosts);
                }
                else
                    return false;
            }
            else
                return false;
            
        }
        else if(inet_pton(AF_INET6, IP.substr(0, IP.find('/')).c_str(), buf6))
        {
            int index = IP.find('/') + 1;
            if(index != 0){
                const char *hosts = IP.substr(index, IP.npos).c_str();

                int nmbHosts = strtol(hosts, NULL, 10);
                if(nmbHosts){
                    // this won't calculate lower subnet number than 65
                    unsigned long long actualHosts =  pow(2, 128 - nmbHosts);

                    printf("%s (%llu hosts)\n",IP.c_str(), actualHosts);

                    // create host range entry
                    IPsubnet(IP.substr(0, IP.find('/')).c_str(), IPv6, nmbHosts);
                }
                else
                    return false;
            }
            else
                return false;
        }
    }

    printf("\n");
    return true;
}