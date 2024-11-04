/**
 * KRY project 1 - Affine cipher
 * @author Timotej Ponek, xponek00
 * @file main.cpp
 */

#include "args.h"
#include "kry.h"

void printHelp(){
    std::cout << "Help:\n\
    \t specification of mode (required):\n\
    \t\t -e -> encrypt\n\
    \t\t -d -> decrypt\n\
    \t\t -c -> decrypt without keys (frequency analysis)\n\
    \t keys - required if mode '-e' or '-d' is selected:\n\
    \t\t -a <val> -> value of 'a' part of key\n\
    \t\t -b <val> -> value of 'b' part of key\n\
    \t file parameters (optional)\n\
    \t\t -f <filename> -> input file (if not specified - stdin)\n\
    \t\t -o <filename> -> output file (if not specified - stdout)\n";
}

int main(int argc, char* argv[]){

    Args args = Args();
	// if args parsed correctly
    if(args.ParseArgs(argc, argv)){
        Kry kry = Kry(args, argv[argc - 1]);
        kry.Run();
    }
    else
        printHelp();

    return 0;
}
