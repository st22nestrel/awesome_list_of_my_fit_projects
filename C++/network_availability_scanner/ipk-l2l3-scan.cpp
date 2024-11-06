/**
 * IPK second project - variant Delta
 * @author Timotej Ponek, xponek00
 * @file main.cpp
 */
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <ifaddrs.h>
#include <string.h>
#include <netinet/ip_icmp.h>
#include <linux/if_arp.h>

#include "args.h"
#include "hosts.h"
#include "IPs.h"
#include "timer.h"

#define PROTO_ARP 0x0806
#define ETH2_HEADER_LEN 14
#define HW_TYPE 1
#define PROTOCOL_TYPE 0x800
#define MAC_LENGTH 6
#define IPV4_LENGTH 4
#define ARP_REQUEST 0x01
#define ARP_REPLY 0x02
#define BUF_SIZE 60

// Define some constants.
#define IP4_HDRLEN 20         // IPv4 header length
#define ICMP_HDRLEN 8         // ICMP header length for echo request, excludes data

//struct to represent arp_header
struct arp_header
{
    unsigned short hardware_type;
    unsigned short protocol_type;
    unsigned char hardware_len;
    unsigned char  protocol_len;
    unsigned short opcode;
    unsigned char sender_mac[MAC_LENGTH];
    unsigned char sender_ip[IPV4_LENGTH];
    unsigned char target_mac[MAC_LENGTH];
    in_addr target_ip;
};

/**
 * @brief Answers to whether array of unsigned chars is zeroed out (empty)
 * @param chr array of unsigned chars
 * @param size portion of array that is expected to be zeroed out
 * @return true if array is full of zeroes, false otherwise
 */
bool uCharEmpty(unsigned char* chr, int size){
    for(int i=0; i<size; i++){
        if(chr[i] != 0)
            return false;
    }
    return true;
}


/**
 * @brief Calculates checksum for ICMP packet
 * function from user liwing https://stackoverflow.com/a/13548622
 * which he shared in comments section to answer https://stackoverflow.com/a/13548622
 * to question https://stackoverflow.com/q/13543554
 * original code from user liwing can be found here https://pastebin.com/62JmMddE
 * @param addr array of unsigned chars
 * @param len portion of array that is expected to be zeroed out
 * @return int calculated checksum
 */
unsigned short int checksum (unsigned short int *addr, int len)
{
    int nleft = len;
    int sum = 0;
    unsigned short int *w = addr;
    unsigned short int answer = 0;
    
    while (nleft > 1) {
        sum += *w++;
        nleft -= sizeof (unsigned short int);
    }
    
    if (nleft == 1) {
        *(unsigned char *) (&answer) = *(unsigned char *) w;
        sum += answer;
    }
    
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    answer = ~sum;
    return (answer);
}


int main(int argc, char* argv[]){

    bool isItOk = Args::ParseArgs(argc, argv);
    Timer::TimerSet(Args::waitTime);

    if(isItOk){
        isItOk = PrintNmbOfHosts(Args::segments);

    // get mac adress of interface
    int sdTmp, ifIndex;
    unsigned char intfBuf[BUF_SIZE];
    struct ifreq ifr;

    memset(intfBuf,0x00,60);

    sdTmp = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sdTmp == -1) {
        perror("socket():");
        exit(1);
    }

    strcpy(ifr.ifr_name, Args::interface.c_str());
    
    /*retrieve interface index*/
    if (ioctl(sdTmp, SIOCGIFINDEX, &ifr) == -1) {
        perror("SIOCGIFINDEX");
        exit(1);
    }
    ifIndex = ifr.ifr_ifindex;

    /*retrieve corresponding MAC*/
    if (ioctl(sdTmp, SIOCGIFHWADDR, &ifr) == -1) {
        perror("SIOCGIFINDEX");
        exit(1);
    }
    close (sdTmp);

    // loop through ranges of IPs to scan
    for(auto IP : IPsubnet::allIPs){
        switch(IP.IPversion){
            case IPv4:
                in_addr start;
                IP.currentIP.ipv4 = IP.IPRangeStart.ipv4;

                while(ntohl(IP.currentIP.ipv4.s_addr) <= ntohl(IP.IPRangeEnd.ipv4.s_addr)){

                    /**
                     * Code bellow is adopted from answer https://stackoverflow.com/a/37268164
                     * by user user6343961 https://stackoverflow.com/users/6343961/user6343961
                     * to qustion how to create arp requst and recieve the response 
                     * https://stackoverflow.com/q/16710040
                     */
                
                    // variables needed for ARP request
                    int sd;
                    unsigned char buffer[BUF_SIZE];
                    unsigned char source_ip[4] = {0,0,0,0};
                    unsigned char target_ip[4] = {192,168,1,1};
                    struct ethhdr *send_req = (struct ethhdr *)buffer;
                    struct ethhdr *rcv_resp= (struct ethhdr *)buffer;
                    struct arp_header *arp_req = (struct arp_header *)(buffer+ETH2_HEADER_LEN);
                    struct arp_header *arp_resp = (struct arp_header *)(buffer+ETH2_HEADER_LEN);
                    struct sockaddr_ll socket_address;
                    int index,ret,length=0;

                    bool arpOK;

                    memset(buffer,0x00,60);

                    for (index = 0; index < 6; index++)
                    {
                        send_req->h_dest[index] = (unsigned char)0xff;
                        arp_req->target_mac[index] = (unsigned char)0x00;
                        /* Filling the source  mac address in the header*/
                        send_req->h_source[index] = (unsigned char)ifr.ifr_hwaddr.sa_data[index];
                        arp_req->sender_mac[index] = (unsigned char)ifr.ifr_hwaddr.sa_data[index];
                        socket_address.sll_addr[index] = (unsigned char)ifr.ifr_hwaddr.sa_data[index];
                    }

                    /*prepare sockaddr_ll*/
                    socket_address.sll_family = AF_PACKET;
                    socket_address.sll_protocol = htons(ETH_P_ARP);
                    socket_address.sll_ifindex = ifIndex;
                    socket_address.sll_hatype = htons(ARPHRD_ETHER);
                    socket_address.sll_pkttype = (PACKET_BROADCAST);
                    socket_address.sll_halen = MAC_LENGTH;
                    socket_address.sll_addr[6] = 0x00;
                    socket_address.sll_addr[7] = 0x00;

                    /* Setting protocol of the packet */
                    send_req->h_proto = htons(ETH_P_ARP);

                    /* Creating ARP request */
                    arp_req->hardware_type = htons(HW_TYPE);
                    arp_req->protocol_type = htons(ETH_P_IP);
                    arp_req->hardware_len = MAC_LENGTH;
                    arp_req->protocol_len =IPV4_LENGTH;
                    arp_req->opcode = htons(ARP_REQUEST);
                    arp_req->target_ip = IP.currentIP.ipv4;

                    // Submit request for a raw socket descriptor.
                    if ((sd = socket (PF_PACKET, SOCK_RAW, htons (ETH_P_ALL))) < 0) {
                        perror ("socket() failed ");
                        exit (EXIT_FAILURE);
                    }

                    // timeout
                    struct timeval timeout;
                    timeout.tv_sec = Args::waitTime/1000;
                    timeout.tv_usec = Args::waitTime%1000;

                    if(setsockopt(sd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
                        perror ("setsockopt() failed to set timeout");
                        exit (EXIT_FAILURE);
                    }
                    
                    buffer[32]=0x00;

                    ret = sendto(sd, buffer, 42, 0, (struct  sockaddr*)&socket_address, sizeof(socket_address));
                    if (ret == -1){
                        perror("sendto():");
                        exit(1);
                    }
                    
                    //delete for recieving
                    memset(buffer,0x00,60);

                    Timer::TimerStart();
                    
                    while(!Timer::HasPassed()){
                        length = recvfrom(sd, buffer, BUF_SIZE, 0, NULL, NULL);
                        if (length == -1){
                            break;
                        }
                        else if(htons(rcv_resp->h_proto) == PROTO_ARP){
                            Timer::TimerStop();
                            break;
                        }
                    }

                    printf("%s\t", inet_ntoa(IP.currentIP.ipv4)); //current scaned ip

                    if(htons(rcv_resp->h_proto) == PROTO_ARP){
                        //we check for "target_mac" because "sender_mac" can sometimes be duplicate from previous call
                        if(uCharEmpty(arp_resp->target_mac, MAC_LENGTH)){
                            printf("arp FAIL, icmpv4 FAIL");
                            arpOK = false;
                        }
                        else {
                            printf("arp OK (");
                            for(index=0;index<6;index++){
                                //"target_mac" was not empty, "sender_mac" is unlikely duplicate
                                printf("%02X",arp_resp->sender_mac[index]);
                                printf(index == 5 ? "" : "-");
                            }
                            printf("), ");
                            arpOK = true;
                        }
                    }
                    else{
                        printf("arp FAIL, icmpv4 FAIL");
                        arpOK = false;
                    }                

                    close(sd);

                /*******************************************************************************************************/
                    
                    // if we got arp response, there is a chance we might get icmp response too
                    if(arpOK){
                        /**
                         * Code bellow is based on code from user liwing https://stackoverflow.com/users/1849688/liwing
                         * which he shared in comments section to answer https://stackoverflow.com/a/13548622
                         * to question https://stackoverflow.com/q/13543554
                         * original code from user liwing can be found here https://pastebin.com/62JmMddE
                         */

                        // variables needed for ICMP ping request
                        int status, datalen, sd;
                        int ip_flags[4];
                        const int on = 1;
                        char src_ip[16] = "0.0.0.0";//doesn't matter
                        struct ip iphdr;
                        struct icmp icmphdr;
                        unsigned char data[(IP_MAXPACKET - IP4_HDRLEN - ICMP_HDRLEN) * sizeof (unsigned char)];
                        memset (data, 0, (IP_MAXPACKET - IP4_HDRLEN - ICMP_HDRLEN) * sizeof (unsigned char));
                        unsigned char packet[IP_MAXPACKET * sizeof (unsigned char)];
                        struct sockaddr_in *ipv4, sin;


                        iphdr.ip_hl = IP4_HDRLEN / 4;
                        iphdr.ip_v = 4;
                        iphdr.ip_tos = 0;
                        iphdr.ip_len = htons (IP4_HDRLEN + ICMP_HDRLEN + datalen);
                        iphdr.ip_id = htons (0);
                        ip_flags[0] = 0;
                        ip_flags[1] = 0;
                        ip_flags[2] = 0;
                        ip_flags[3] = 0;
                        iphdr.ip_off = htons ((ip_flags[0] << 15) + (ip_flags[1] << 14) + (ip_flags[2] << 13) + ip_flags[3]);
                        iphdr.ip_ttl = 255;
                        iphdr.ip_p = IPPROTO_ICMP;
                        inet_pton (AF_INET, src_ip, &(iphdr.ip_src));
                        
                        iphdr.ip_dst = IP.currentIP.ipv4;
                        
                        // IPv4 header checksum (16 bits): set to 0 when calculating checksum
                        iphdr.ip_sum = 0;
                        iphdr.ip_sum = checksum ((unsigned short int *) &iphdr, IP4_HDRLEN);


                        // ICMP header                           
                        // Message Type (8 bits): echo request
                        icmphdr.icmp_type = ICMP_ECHO;
                        // Message Code (8 bits): echo request
                        icmphdr.icmp_code = 0;
                        // Identifier (16 bits): usually pid of sending process - pick a number
                        icmphdr.icmp_id = htons (1000);
                        // Sequence Number (16 bits): starts at 0
                        icmphdr.icmp_seq = htons (0);
                        // ICMP header checksum (16 bits): set to 0 when calculating checksum
                        icmphdr.icmp_cksum = 0;
                        
                        // Prepare packet.
                        // Next part of packet is upper layer protocol header.
                        memcpy ((packet ), &icmphdr, ICMP_HDRLEN);
                        // Finally, add the ICMP data.
                        memcpy (packet  + ICMP_HDRLEN, data, datalen);
                        
                        // Calculate ICMP header checksum
                        icmphdr.icmp_cksum = checksum ((unsigned short int *) (packet ), ICMP_HDRLEN + datalen);
                        memcpy ((packet ), &icmphdr, ICMP_HDRLEN);
                        

                        memset (&sin, 0, sizeof (struct sockaddr_in));
                        sin.sin_family = AF_INET;
                        sin.sin_addr.s_addr = iphdr.ip_dst.s_addr;
                        
                        // Submit request for a raw socket descriptor.
                        if ((sd = socket (AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0) {
                            perror ("socket() failed ");
                            exit (EXIT_FAILURE);
                        }
                        
                                                    
                        // Bind socket to interface index
                        // this is optional but increases performance
                        if (setsockopt (sd, SOL_SOCKET, SO_BINDTODEVICE, &ifr, sizeof (ifr)) < 0) {
                            perror ("setsockopt() failed to bind to interface ");
                            exit (EXIT_FAILURE);
                        }
                        bind(sd,(struct sockaddr*)&sin,sizeof(sin));

                        // timeout
                        struct timeval timeout;
                        timeout.tv_sec = Args::waitTime/1000;
                        timeout.tv_usec = Args::waitTime%1000;

                        if(setsockopt(sd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
                            perror ("setsockopt() failed to set timeout");
                            exit (EXIT_FAILURE);
                        }

                        // Send packet.
                        if (sendto (sd, packet, ICMP_HDRLEN + datalen, 0, (struct sockaddr *) &sin, sizeof (struct sockaddr)) < 0)  {
                            perror ("sendto() failed ");
                            exit (EXIT_FAILURE);
                        }

                        struct sockaddr_in rec;
                        unsigned char pkt[2048];
                        
                        Timer::TimerStart();
                        int replyCode;

                        while(!Timer::HasPassed()){
                            if (recvfrom (sd, (void*)pkt, sizeof(struct ip) + sizeof(struct icmp)+datalen , 0, NULL, (socklen_t*)sizeof (struct sockaddr)) < 0)  {
                                break;
                            }
                            struct ip *ip = (struct ip *)pkt;
                            struct icmp *icmp = (struct icmp *)(pkt + sizeof(struct ip));
                            
                            replyCode = icmp->icmp_code;
                            if(replyCode == ICMP_DEST_UNREACH || replyCode == ICMP_ECHOREPLY){
                                Timer::TimerStop();
                                break;
                            }
                        }

                        if(replyCode == ICMP_PROT_UNREACH || replyCode == ICMP_ECHOREPLY){
                            printf("icmpv4 OK");
                        }
                        else
                            printf("icmpv4 FAIL");

                        close (sd);
                        
                    /*********************************************************************************************************************************/
                    }
                    
                    printf("\n");  

                    IP.currentIP.ipv4.s_addr = htonl(ntohl(IP.currentIP.ipv4.s_addr)+1);
                    
                }

                break;
            
            case IPv6:
                //Not implemented
                break;
            }

        }

    }
        
}