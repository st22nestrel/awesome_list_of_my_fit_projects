/**
 * KRY project 1 - Affine cipher
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
    bool encrypt = false;
    bool decrypt = false;
    bool freqAnalysis = false;

    std::string inputFile;
    std::string outputFile;
    
    int a = 0;
    int b = 0;

public:
    Args();
    bool ParseArgs(int argc, char* argv[]);
};