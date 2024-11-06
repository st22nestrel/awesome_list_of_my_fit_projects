/**
 * ISA project - TFTP client
 * @author Timotej Ponek, xponek00
 * @file args.cpp
 */
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <ifaddrs.h>
#include <string>
#include <vector>
#include "utils.h"
#include "args.h"

Args::Args(){
}

void Args::ClearArgs(){
    optind = 1;
    directory = "";
    timeout = "";
    segment = "";
    mode = "";
    multiCast = false;
    write = false;
    read = false;
}

/**
 * @brief Parses arguments
 * @param argc 
 * @param argv
 * @return true if all needed arguments were set, false otherwise
 */
bool Args::ParseArgs(std::string input)
{
    ClearArgs();

    std::vector<std::string> splitArgs = split(input, ' ');

    char **argv = new char*[splitArgs.size() + 1];
    argv[0] = (char*) "TFTP_client";

    for (int i = 1; i < splitArgs.size() + 1; i++)
    {
        argv[i] = (char*) splitArgs[i - 1].c_str();
    }

    bool retVal = ParseArgs(splitArgs.size() + 1, argv);
    delete[] argv;

    return retVal;
}

/**
 * @brief Parses arguments
 * @param argc 
 * @param argv
 * @return true if all needed arguments were set, false otherwise
 */
bool Args::ParseArgs(int argc, char* argv[])
{
    this->addressAndPort = "127.0.0.1";

    int c;

    while ((c = getopt_long(argc, argv, "RWd:t:s:a:c:m", NULL, NULL)) != -1)
    switch(c){
        case 'R':
            if(!write)
                read = true;
            else{
                fprintf(stderr, "Cannot se both read and write");
                return false;
            }
        break;

        case 'W':
            if(!read)
                write = true;
            else{
                fprintf(stderr, "Cannot se both read and write");
                return false;
            }   
        break;

        case 'd':
            if(optarg == NULL){
                fprintf(stderr, "directory missing");
                return false;
            }
            else{
                Args::directory = optarg;
            }
        break;

        // optional
        case 't':
        {
            int timeout = std::stoi(optarg);
            if(timeout < 1 || timeout > 255){
                fprintf(stderr, "Invalid timeout");
                return false;
            }
            else{
                Args::timeout = optarg;
            }
        }
        break;

        // optional
        case 's':
        {
            int segment = std::stol(optarg);
            if(segment < 8 || segment > 65464){
                fprintf(stderr, "Invalid segment lenght");
                return false;
            }
            else{
                Args::segment = optarg;
            }
        }
            break;
        
        // optional
        case 'm':
            Args::multiCast = true;
            break;
        
        // optional
        case 'c':
            Args::mode = optarg;
            break;

        // optional
        case 'a':
            Args::addressAndPort = optarg;
            break;
        // ignore undefined args
        default:
            continue;
    }
    
    if(Args::read == Args::write)
        return false;

    if(Args::directory.empty())
        return false;

    if(Args::mode.empty())
        Args::mode = "binary";

    return true;
}