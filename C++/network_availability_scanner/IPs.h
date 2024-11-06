/**
 * IPK second project - variant Delta
 * @author Timotej Ponek, xponek00
 * @file IPs.h
 */
#include <vector>
#include <array>
#include <string>
#include <arpa/inet.h>

#define IPv4 1
#define IPv6 2

/**
 * Class holds all IP ranges to be scanned in static member vector
 * code in this class is based on asnwer https://codereview.stackexchange.com/a/141840
 * from user 200_success (https://codereview.stackexchange.com/users/9357/200-success)
 * to question: https://codereview.stackexchange.com/q/141674
 */
class IPsubnet
{
private:
    // represents one subnet to be scanned
    struct IP
    {
        std::string begin;
        std::string end;
        union{
            in_addr ipv4;
            in6_addr ipv6;
        }IPRangeStart;
        union{
            in_addr ipv4;
            in6_addr ipv6;
        }IPRangeEnd;
        union{
            in_addr ipv4;
            in6_addr ipv6;
        }currentIP;
        int IPversion;
    };
public:
    static std::vector<IP> allIPs; //Vector with all subnet ranges
    IPsubnet(const char* ip, int ipVersion, int subnet);
    ~IPsubnet();
};