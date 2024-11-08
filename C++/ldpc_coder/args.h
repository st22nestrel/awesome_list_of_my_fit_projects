/**
 * BMS project 1 - LDPC coder
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
    bool encode = false;
    bool decode = false;

    std::string matrixFile;

public:
    Args();
    bool ParseArgs(int argc, char *argv[]);
};