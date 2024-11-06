/**
 * ISA project - TFTP client
 * @author Timotej Ponek, xponek00
 * @file tftp.h
 */

#include <sys/time.h>
#include <string.h>
#include "utils.h"
#include "args.h"
#include "datablocks.h"
#include "tftp.h"

/**
 * Transfers file from its format to netascii
 */
void Tftp::FileToNetascii(){
    
    FILE *new_file = fopen(".ignoreME", "wb");
    FILE *read_file = fopen(args->directory.c_str(), "rb");

    int c = 0;

    //CR - 13
    //LF - 10

    while (fread(&c, sizeof(char), 1, read_file) != 0 && c != EOF){
        
        //LF : put CR->LF
        if(c == 10){
            fputc(13, new_file);
        }

        fputc(c, new_file);

        //CR : put CR->NUL
        if(c == 13){
            fputc(0, new_file);
        }
    }

    fclose(new_file);
    fclose(read_file);
}

/**
 * Transfers file from netascii format to ascii
 */
void Tftp::FileFromNetascii(){
    
    FILE *new_file = fopen(".ignoreME", "wb+");
    FILE *read_file = fopen(args->directory.c_str(), "rb+");

    char c = 0;

    //CR - 13
    //LF - 10

    while (fread(&c, sizeof(char), 1, read_file) != 0 && c != EOF){
        
        // we have LL, delete CR
        if(c == 10){
            fseek(new_file, -1, SEEK_CUR);
            fputc(c, new_file);
        }

        fputc(c, new_file);

        // we have CR, skip one char and save following
        if(c == 13){
            fread(&c, sizeof(char), 1, read_file);//Nul
            fread(&c, sizeof(char), 1, read_file);//Some ch
            fputc(c, new_file);
        }
    }

    fclose(new_file);
    fclose(read_file);

    remove(args->directory.c_str());
    rename(".ignoreME", args->directory.c_str());
}


/**
 * @brief Constructor
 * @param arguments program arguments
 */
Tftp::Tftp(Args &arguments){
    args = &arguments;
    isMaster = false;
    createNewBuffer = false;
    buffer = nullptr;
}

/**
 * @brief Prints error to stderr, including timestamp
 * @param string message to print
 * @param pointer value to put into message (to %s format specifier)
 */
void Tftp::printError(std::string string, char *pointer){
    //function to get time adopted from https://bytes.com/topic/c/answers/550366-getting-current-time-milliseconds#post3715465
    gettimeofday(&tp, 0);
    tm *t = localtime(&tp.tv_sec);
    fprintf(stderr, "[%d-%02d-%d %02d:%02d:%02d.%ld]\t", t->tm_year + 1900, t->tm_mday + 1, t->tm_mon + 1, t->tm_hour, t->tm_min, t->tm_sec, tp.tv_usec/1000);
    fprintf(stderr, string.c_str(), pointer);
}

/**
 * @brief Prints message to stdout, including timestamp
 * @param string message to print
 * @param pointer value to put into message (to %s format specifier)
 */
void Tftp::printMsg(std::string string, char *pointer){
    gettimeofday(&tp, 0);
    tm *t = localtime(&tp.tv_sec);
    fprintf(stdout, "[%d-%02d-%d %02d:%02d:%02d.%ld]\t", t->tm_year + 1900, t->tm_mday + 1, t->tm_mon + 1, t->tm_hour, t->tm_min, t->tm_sec, tp.tv_usec/1000);
    fprintf(stdout, string.c_str(), pointer);
}

/**
 * @brief Prints transfer state to stdout, including timestamp
 * @param string message to print
 */
void Tftp::printTransferState(std::string string){
    gettimeofday(&tp, 0);
    tm *t = localtime(&tp.tv_sec);
    fprintf(stdout, "[%d-%02d-%d %02d:%02d:%02d.%ld]\t", t->tm_year + 1900, t->tm_mday + 1, t->tm_mon + 1, t->tm_hour, t->tm_min, t->tm_sec, tp.tv_usec/1000);
    fprintf(stdout, string.c_str(), transferedSize, fileSize);
}


/**
 * @brief Sends error code to given port on server
 * @param ErrCode error code
 */
void Tftp::sendErrorPacket(int ErrCode){
    *(short *)buffer = htons(OP_ERROR);	// The op-code
    p = buffer + 2;
    *(short *)p = htons(ErrCode); // unknown tid
    
    // send packet to "wrong" port
    count = sendto(sock, buffer, p-buffer, 0, (struct sockaddr *)&serverAddress, sizeof serverAddress);
}


/**
 * @return true if port is bad, else false;
 */
bool Tftp::CheckPort(){
    if(serverAddress.sin_port != agreedPort){
        sendErrorPacket(5);
        return true;
    }
    return false;
}

/**
 * @brief answers to whether ip address in arguments is IPv6 or not
 * side effect - parses ip address in args to correct in_addr struct
 * @return whether parsed ip address is IPv6
 */
bool Tftp::isIPv6()
{
    if(!inet_pton(AF_INET6, args->addressAndPort.c_str(), &IPv6address))
    {
        inet_pton(AF_INET, args->addressAndPort.c_str(), &IPv4address);
        return false;
    }
    else
        return true;
}

/**
 * @brief answers to whether ip address in arguments is IPv4 or not
 * side effect - parses ip address in args to correct in_addr struct
 * @return whether parsed ip address is IPv4
 */
bool Tftp::isIPv4(){
    return isIPv4(args->addressAndPort);
}

/**
 * @brief answers to whether ip address in arguments is IPv4 or not
 * side effect - parses given ip address to correct in_addr struct
 * @param address string containing address to parse
 * @return whether parsed ip address is IPv4
 */
bool Tftp::isIPv4(std::string &address)
{
    if(!inet_pton(AF_INET, address.c_str(), &IPv4address)) 
    {
        inet_pton(AF_INET6, address.c_str(), &IPv6address);
        return false;
    }
    else
        return true;
}

/**
 * @brief Creates UDP socket and extracts ip address and port
 */
void Tftp::CreateSocket(){
    // common UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    std::string address = args->addressAndPort;

    // if no port specified, use default port, else use specified port
    if(args->addressAndPort.find(',') == args->addressAndPort.npos)
        serverAddress.sin_port = htons(TFTP_PORT);
    else{
        address = args->addressAndPort.substr(0, args->addressAndPort.find(','));
        serverAddress.sin_port = htons(strtol(args->addressAndPort.substr(args->addressAndPort.find(',') + 1, -1).c_str(), NULL, 10));
    }   

    if(isIPv4(address)) {
        serverAddress.sin_family      = AF_INET;
        serverAddress.sin_addr.s_addr = IPv4address.s_addr;
    }
    else {
        serverAddress.sin_family      = AF_INET6;
        serverAddress.sin_addr.s_addr = IPv4address.s_addr;
    }
}

/**
 * Inserts needed info to buffer to create read request
 */
void Tftp::CreateReadRequest(){
    *(short *)buffer = htons(OP_RRQ);	/* The op-code   */
    p = buffer + 2;
    strcpy(p, args->directory.c_str());		/* The file name */
    p += args->directory.length() + 1;	    /* Keep the nul  */
    strcpy(p, args->mode.c_str());			/* The Mode */
    p += args->mode.length() + 1;
}

/**
 * @return true if client has options specified, false otherwise
 */
bool Tftp::HasOptions(){
    return (!args->segment.empty() || !args->timeout.empty() || args->multiCast);
}

/**
 * @brief Inserts options specified via cmd to buffer
 * option tsize is included in every first packet
 */
void Tftp::CreateOptions(){
    if(!args->timeout.empty()){
        strcpy(p, "timeout");
        p += 8;
        strcpy(p, args->timeout.c_str());
        p += args->timeout.length() + 1;
    }

    if(!args->segment.empty()){
        strcpy(p, "blksize");
        p += 8;
        strcpy(p, args->segment.c_str());
        p += args->segment.length() + 1;
    }

    if(args->multiCast){    
        strcpy(p, "multicast");
        p += 10;
        strcpy(p, "");
        p += 1;
    }

    if(1){
        strcpy(p, "tsize");
        p += 6;
        if(args->read){
            strcpy(p, "0");	// placeholder size
            p += 2;
        }
        else{
            if(args->mode == "netascii"){
                FileToNetascii();
                
                args->directory = ".ignoreME";
            }

            std::string fileSize = getFileSize(args->directory.c_str(), this->fileSize);

            //we do this so the server can acknowledge
            strcpy(p, fileSize.c_str());
            p += fileSize.length() + 1;
        }       
    }
}

/**
 * Creates new variable size buffer, deleting the old one
 */
void Tftp::CreateNewBuffer(){
    if(createNewBuffer){
        delete[] buffer;
        buffer = new char[packetSize];
        createNewBuffer = false;
    }
}

/**
 * Gets option from buffer, if there is any
 * @return true if any option was found, false otherwise
 */
bool Tftp::GetOption(){

    if(!args->timeout.empty() && !strcmp(p, "timeout")){
        p += 8;
        timeout = std::stoi(p);
        if (strcmp(p, args->timeout.c_str())){
            printError("Using timeout %s s, which differs from the timeout specified.\n", p);
        }
        p += strlen(p) + 1;

        return true;
    }
    else if(!args->segment.empty() && !strcmp(p, "blksize")){
        p += 8;
        packetSize = std::stol(p) + 4;
        p += strlen(p) + 1;
        createNewBuffer = true;

        return true;
    }
    else if(args->multiCast && !strcmp(p, "multicast")){
        p += 8;

        std::vector<std::string> splitArgs = split(p, ',');

        if(isIPv4(splitArgs[0])){
            serverAddress.sin_family      = AF_INET;
            serverAddress.sin_addr.s_addr = IPv4address.s_addr;
        }
        else {
            serverAddress.sin_family      = AF_INET6;
            serverAddress.sin_addr.s_addr = IPv4address.s_addr;
        }

        serverAddress.sin_port = htons(strtol(splitArgs[1].c_str(), NULL, 10));

        agreedPort = serverAddress.sin_port;

        int master = strtol(splitArgs[2].c_str(), NULL, 10);

        master = master ? true : false;

        p += strlen(p) + 1;

        return true;
    }
    else if(!strcmp(p, "tsize")){
        p += 6;
        if(args->read)
            fileSize = std::stol(p);
        p += strlen(p) + 1;

        return true;
    }

    return false;
}


/**
 * Send RRQ to server, recieves answer and then proceeds
 * @returns
 * false:
 * - if error was encountered, transfer ends with error
 * - if filesize was less then tsize(data portion of packet), transfer ends succesfully without proceeding further
 * true:
 * - otherwise false is returned to specify that transfer should proceed to RecievePackets() function
 */
bool Tftp::SentFirstPacket_SetOptions_SetPort(){
    count = sendto(sock, buffer, p-buffer, 0, (struct sockaddr *)&serverAddress, sizeof serverAddress);

    // clear buffer
    memset(buffer, 0x00, packetSize);

    // recieve first packet
    server_len = sizeof serverAddress;
    count = recvfrom(sock, buffer, packetSize, 0, (struct sockaddr *)&serverAddress, &server_len);

    if (ntohs(*(short *)buffer) == OP_ERROR || ntohs(*(short *)buffer) == OP_OACK_ERR){
        printError("Error recieving data: %s\n", buffer+4);
        return 1; 
    }

    p = buffer;
    agreedPort = serverAddress.sin_port; //set port

    //Check for options if there are any
    if(ntohs(*(short *)buffer) == OP_OACK){
        p += 2;

        while (GetOption());
        CreateNewBuffer();
        
        // we always accept OACK
        *(short *)buffer = htons(OP_ACK);
        p = buffer + 2;
        *(short *)p = htons(0);
        p += 2;

        // send ack "always"
        count = sendto(sock, buffer, p-buffer, 0, (struct sockaddr *)&serverAddress, sizeof serverAddress);

        p = buffer;

        //no multicast
        if(!args->multiCast || isMaster || multicast.empty()){

            // got ack, take first data packet
            do{
                count = recvfrom(sock, buffer, packetSize, 0, (struct sockaddr *)&serverAddress, &server_len);

                if (ntohs(*(short *)buffer) == OP_ERROR){
                    printError("Error recieving data: %s\n", buffer+4);
                    return 1; 
                }             
            }while (CheckPort());
            
        }

        else{
            return RecievePacketsMulticast();
        }
    }
    
    file = fopen(args->directory.c_str(), "wb");

    fwrite(buffer+4, count-4, 1, file);

    transferedSize += count - 4;
    printTransferState("Transfered %lu B of %lu B\n");

    *(short *)buffer = htons(OP_ACK);
    sendto(sock, buffer, 4, 0, (struct sockaddr *)&serverAddress, sizeof serverAddress);

    if(!(count == packetSize)){
        fclose(file);
        if(args->mode == "netascii")
            FileFromNetascii();
        return 1;
    }
    
    return 0;
}


/**
 * Recieve data packets from server, until the whole file is transfered
 * @returns true if no error was encountered, false otherwise
 */
bool Tftp::RecievePackets(){
    server_len = sizeof serverAddress;

    do {
        count = recvfrom(sock, buffer, packetSize, 0, (struct sockaddr *)&serverAddress, &server_len);
        
        if (ntohs(*(short *)buffer) == OP_ERROR) {
            printError("Error recieving data: %s\n", buffer+4);
            fclose(file);
            return false;
        }
        else if(serverAddress.sin_port != agreedPort){
            sendErrorPacket(5);
        }
        else {
            fwrite(buffer+4, count-4, 1, file);

            transferedSize += count - 4;
            printTransferState("Transfered %lu B of %lu B\n");

            *(short *)buffer = htons(OP_ACK);
            sendto(sock, buffer, 4, 0, (struct sockaddr *)&serverAddress,
                    sizeof serverAddress);
        }
    } while (count == packetSize);

    fclose(file);

    if(args->mode == "netascii")
        FileFromNetascii();

    return true;
}


/**
 * Recieve data packets from server on multicast adress, until the whole file is transfered
 * @returns true if no error was encountered, false otherwise
 */
bool Tftp::RecievePacketsMulticast(){
    DataBlocks dataBlocks = DataBlocks(fileSize);

    while (!dataBlocks.isDownloaded())
    {
        // clear buffer
        memset(buffer, 0x00, packetSize);

        // got ack, take first data packet
        count = recvfrom(sock, buffer, packetSize, 0, (struct sockaddr *)&serverAddress, &server_len);

        if (ntohs(*(short *)buffer) == OP_ERROR || CheckPort()){
            //we are not master, so we ignore
            continue;
        }
        if (ntohs(*(short *)buffer) == OP_OACK){
            while (GetOption());
            CreateNewBuffer();

            if(!isMaster){
                continue;
            }
            else{
                // send ack
                *(short *)buffer = htons(OP_ACK);
                p = buffer + 2;
                *(short *)p = htons(0);
                p += 2;

                count = sendto(sock, buffer, p-buffer, 0, (struct sockaddr *)&serverAddress, sizeof serverAddress);

                do{
                    count = recvfrom(sock, buffer, packetSize, 0, (struct sockaddr *)&serverAddress, &server_len);

                    if (ntohs(*(short *)buffer) == OP_ERROR){
                        printError("Error recieving data: %s\n", buffer+4);
                        return 1; 
                    }             
                }while (CheckPort());
                
                do{
                    short blockNmb = dataBlocks.GetFirstMissing();

                    // safety terminator
                    buffer[count] = '\0';

                    transferedSize = dataBlocks.getDataBlockSize();
                    printTransferState("Transfered %lu B of %lu B");

                    *(short *)buffer = htons(OP_ACK);
                    p = buffer + 2;
                    *(short *)p = htons(blockNmb);
                    p += 2;

                    sendto(sock, buffer, 4, 0, (struct sockaddr *)&serverAddress,
                            sizeof serverAddress);

                    do{
                        count = recvfrom(sock, buffer, packetSize, 0, (struct sockaddr *)&serverAddress, &server_len);

                        if (ntohs(*(short *)buffer) == OP_ERROR){
                            printError("Error recieving data: %s\n", buffer+4);
                            return 1; 
                        }             
                    }while (CheckPort());

                    dataBlocks.AddDataBlock(ntohs(*(short *)(buffer+2)), buffer+4);

                } while (!dataBlocks.isDownloaded());

                
            }
        }
        // sidenote: if blockNumber overflows, we cannot say where to put incoming block
        dataBlocks.AddDataBlock(ntohs(*(short *)(buffer+2)), buffer+4);

    }

    file = fopen(args->directory.c_str(), "wb");
    dataBlocks.fprintfDataBlocks(file);
    fclose(file);

    return 1;
}

/**
 * Used for simulating multicast
 */
void Tftp::SendAck(){
    *(short *)buffer = htons(OP_ACK);
    sendto(sock, buffer, 4, 0, (struct sockaddr *)&serverAddress,
        sizeof serverAddress);
}


/**
 * Inserts needed info to buffer to create write request
 */
void Tftp::CreateWriteRequest(){
    *(short *)buffer = htons(OP_WRQ);	/* The op-code   */
    p = buffer + 2;
    strcpy(p, args->directory.c_str());		/* The file name */
    p += args->directory.length() + 1;	    /* Keep the nul  */
    strcpy(p, args->mode.c_str());			/* The Mode */
    p += args->mode.length() + 1;
}


/**
 * Send WRQ to server, recieves answer and then proceeds
 * @returns if error was encountered, false otherwise
 */
bool Tftp::WRQ_SentFirstPacket_SetOptions_SetPort(){
    count = sendto(sock, buffer, p-buffer, 0, (struct sockaddr *)&serverAddress, sizeof serverAddress);

    // clear buffer
    memset(buffer, 0x00, packetSize);

    // recieve first packet
    server_len = sizeof serverAddress;
    count = recvfrom(sock, buffer, packetSize, 0, (struct sockaddr *)&serverAddress, &server_len);

    if (ntohs(*(short *)buffer) == OP_ERROR || ntohs(*(short *)buffer) == OP_OACK_ERR){
        printError("Error sending data: %s\n", buffer+4);
        return 1; 
    }

    p = buffer;
    agreedPort = serverAddress.sin_port;

    //Check for options if there are any
    if(ntohs(*(short *)buffer) == OP_OACK){
        p += 2;

        while (GetOption());
        CreateNewBuffer();
    }

    f.open(args->directory.c_str());

    return 0;
}


/**
 * Recieve data packets to server, until the whole file is transfered
 * @returns true if no error was encountered, false otherwise
 */
bool Tftp::SendPackets(){
    server_len = sizeof serverAddress;

    int dataPacketCounter = 1;
    char* helperbuffer = new char[packetSize - 4];

    bool sent;

    do{
        f.read(helperbuffer, packetSize - 4);

        do{
            *(short *)buffer = htons(OP_DATA);
            p = buffer + 2;
            *(short *)p = htons(dataPacketCounter);
            p += 2;

            memcpy(p, helperbuffer, packetSize - 4);

            int why = f.gcount();
            count = sendto(sock, buffer, f.gcount() + 4, 0, (struct sockaddr *)&serverAddress, sizeof serverAddress);

            //wait until we focked
            while(1){
                count = recvfrom(sock, buffer, packetSize, 0, (struct sockaddr *)&serverAddress, &server_len);

                if (ntohs(*(short *)buffer) == OP_ERROR) {
                    printError("Error Sending data: %s\n", buffer+4);
                    delete[] helperbuffer;
                    return false;
                }
                else if(serverAddress.sin_port != agreedPort){
                    sendErrorPacket(5);
                }
                else{
                    sent = true;
                    break;
                }
            }
        }while(!sent);

        transferedSize += f.gcount();
        printTransferState("Transfered %lu B of %lu B\n");
        sent = false;
        dataPacketCounter++;

    } while (transferedSize < fileSize);

    f.close();

    if(args->mode == "netascii")
        remove(".ignoreME");

    delete[] helperbuffer;

    return true;
}


/**
 * Starting point of TFTP client
 * client logic is inspired by implementation from
 * https://www.linkedin.com/pulse/tftp-client-implementation-c-sumit-jha/
 * @return true if transfer ended succesfully, false otherwise
 */
bool Tftp::tftpClient(){

    //reset packet size
    packetSize = 516;
    transferedSize = 0;
    //reset multicast option string
    multicast = "";

    //create buffer on heap
    if(buffer != nullptr){
        delete[] buffer;
    }
    buffer = new char[packetSize];

    CreateSocket();

    if(args->read)
    {
        CreateReadRequest();
        CreateOptions();

        if(!SentFirstPacket_SetOptions_SetPort())
            return RecievePackets();
    }
    else if(args->write)
    {
        CreateWriteRequest();
        CreateOptions();

        if(!WRQ_SentFirstPacket_SetOptions_SetPort())
            return SendPackets();
    }

    return true;
}