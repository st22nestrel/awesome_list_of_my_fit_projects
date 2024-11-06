/**
 * IPK second project - variant Delta
 * @author Timotej Ponek, xponek00
 * @file hosts.h
 */
#include <string>
#include <vector>

/**
 * @brief Prints number of hosts for given subnets (both IPv4 and IPv6)
 * @param IPs vector with all subnets given as launch arguments
 * @return bool true if nmbs of hosts were succesfully extracted from subnets, false on failure
 */
bool PrintNmbOfHosts(std::vector<std::string> IPs);