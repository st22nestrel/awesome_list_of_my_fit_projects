/**
 * ISA project - TFTP client
 * @author Timotej Ponek, xponek00
 * @file tftp.h
 */

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <fstream>

#define TFTP_PORT	69	/* tftp's well-known port number */

#define OP_RRQ		1	/* TFTP op-codes */
#define OP_WRQ		2
#define OP_DATA		3
#define OP_ACK		4
#define	OP_ERROR	5
#define OP_OACK     6
#define OP_OACK_ERR 8

//forward decl
class Args;

/**
 * Class representing Tftp client
 * client logic is inspired by implementation from 
 * https://www.linkedin.com/pulse/tftp-client-implementation-c-sumit-jha/
 */
class Tftp
{

private:
    int    sock;
    struct sockaddr_in serverAddress;		/* Server's address  */
    in_addr  IPv4address;
    in6_addr IPv6address;

    char   *buffer, *p;
    int    count;
    in_port_t agreedPort;
    socklen_t server_len;

    FILE *file = NULL;
    std::ifstream f;

    Args *args;
    timeval tp;
    unsigned long fileSize;
    unsigned long transferedSize;

    int timeout;
    int packetSize;
    std::string multicast;
    bool isMaster;
    bool createNewBuffer;

    void FileToNetascii();
    void FileFromNetascii();

    void printError(std::string string, char *pointer);
    void printMsg(std::string string, char *pointer);
    void printTransferState(std::string string);

    void sendErrorPacket(int ErrCode);
    bool CheckPort();

    bool isIPv6();
    bool isIPv6(std::string &address);
    bool isIPv4();
    bool isIPv4(std::string &address);

    bool HasOptions();
    void CreateOptions();
    void CreateNewBuffer();
    bool GetOption();

    void CreateSocket();

    void CreateReadRequest();
    bool SentFirstPacket_SetOptions_SetPort();
    bool RecievePackets();

    bool RecievePacketsMulticast();

    void SendAck();

    void CreateWriteRequest();
    bool WRQ_SentFirstPacket_SetOptions_SetPort();
    bool SendPackets();

public:
    Tftp(Args &arguments);

    bool tftpClient();
};