/**
 * IPK second project - variant Delta
 * @author Timotej Ponek, xponek00
 * @file args.cpp
 */
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <ifaddrs.h>
#include "args.h"

//forward declarations of static members
std::string Args::interface;
int Args::waitTime;
std::vector<std::string> Args::segments;

/**
 * @brief Parses arguments
 * @param argc 
 * @param argv
 * @return true if all needed arguments were set, false otherwise
 */
bool Args::ParseArgs(int argc, char* argv[])
{
    Args::waitTime = 0;
    
    static struct option long_options[] =
    {
        {"interface", optional_argument, 0, 'i'},
        {"wait", required_argument, 0, 'w'},
        {"subnet", required_argument, 0, 's'},
        {0, 0, 0, 0}
    };

    int c;

    while ((c = getopt_long(argc, argv, "i::w:s:", long_options, NULL)) != -1)
    switch(c){
        case 'i':
            if(Args::interface.empty() && argv[optind] != NULL)
            {
                Args::interface.assign(argv[optind]);
            }
            else if(argv[optind] == NULL)
            {
                PrintInterfaces();
                return false;
            }
            else
                return false;

            break;

        // optional
        case 'w':
            if(!Args::waitTime){
                Args::waitTime = std::stoi(optarg);
                if(Args::waitTime < 1)
                    return false;
            }
            break;

        case 's':
            Args::segments.push_back(optarg);
            break;

        // ignore undefined args
        default:
            continue;
    }

    // default value
    if(!Args::waitTime)
        Args::waitTime = 5000;

    // list available interfaces when -i option is not specified
    if(Args::interface.empty())
    {
        PrintInterfaces();
        return false;
    }
    // no segments given
    else if(Args::segments.empty())
        return false;
    else
        return true;   
}

/**
 * @brief Prints all available interfaces to stdout
 * Function based on code from user edufelipe (https://gist.github.com/edufelipe)
 * original code: https://gist.github.com/edufelipe/6108057
 */
void Args::PrintInterfaces(){
    struct ifaddrs *ifaddr, *ifa;

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        exit(1);
    }
    
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        /* this ensures that single device is not listed multiple times */
        if (ifa->ifa_addr == NULL ||
            ifa->ifa_addr->sa_family != AF_PACKET) continue;

        printf("%s\n", ifa->ifa_name);
    }
}