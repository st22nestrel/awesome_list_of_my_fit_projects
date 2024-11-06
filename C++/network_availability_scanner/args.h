/**
 * IPK second project - variant Delta
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
    static std::string interface;
    static int waitTime;
    static std::vector<std::string> segments;

public:
    static bool ParseArgs(int argc, char* argv[]);
private:
    static void PrintInterfaces();
};