/**
 * ISA project - TFTP client
 * @author Timotej Ponek, xponek00
 * @file main.cpp
 */

#include <iostream>
#include "tftp.h"
#include "args.h"

void printHelp(){
    std::cout << "Help:\n";
    std::cout <<  "\t -R or -W (required) -> read or write operation\n";
    std::cout <<  "\t -d (required) -> file or path, from where the file will be downloaded (-R)\n";
    std::cout <<  "\t \t or where the file will be uploaded (-W)\n";
    std::cout <<  "\t -t (optional) -> timeout for each paket requested from server\n";
    std::cout <<  "\t -s (optional) -> transfer size (custom size of data packet portion) requested from server\n";
    std::cout <<  "\t -m (optional) -> request for multicast from server (available only for -R option)\n";
    std::cout <<  "\t -c (optional) -> mode for data transfer 'ascii'('netascii') or 'binary'('octet')\n";
    std::cout <<  "\t -a (optional) -> IPv4 or IPv6 address with port in format 'adresss,port'\n";
    std::cout <<  "\t \t if port is not specified (i.e format is 'address'), default port 69 is used\n";
}

int main(){
    Args args = Args();

    std::string arguments;

    Tftp tftp = Tftp(args);

    while (true)
    {
        std::cout << "<" << "\n";
        std::cin.clear();
        std::getline(std::cin, arguments);

        if(args.ParseArgs(arguments))
            tftp.tftpClient();
        else
            printHelp();
    }
    return 0;
}