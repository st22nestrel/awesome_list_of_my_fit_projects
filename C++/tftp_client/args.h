/**
 * ISA project - TFTP client
 * @author Timotej Ponek, xponek00
 * @file args.h
 */
#include <string>
#include <vector>

/**
 * Class that provides arguments parsing and stores them as static class members
 */
class Args
{
public:
    //read and write cannot be set at the same time
    bool write;
    bool read;

    std::string directory;
    std::string timeout;
    std::string segment;
    bool multiCast;
    std::string mode;
    
    std::string addressAndPort;

public:
    Args();
    bool ParseArgs(std::string);
    bool ParseArgs(int argc, char* argv[]);

private:
    void PrintInterfaces();
    void ClearArgs();
};