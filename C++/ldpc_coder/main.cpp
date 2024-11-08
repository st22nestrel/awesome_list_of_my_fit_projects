/**
 * BMS project 1 - LDPC coder
 * @author Timotej Ponek, xponek00
 * @file main.cpp
 */

#include "args.h"
#include "bms.h"

void printHelp(){
    std::cout << "Help:\n\
    \t specification of mode (required):\n\
    \t\t -e -> encode - input can be only ASCII characters\n\
    \t\t -d -> decode - only '1' and '0' chars are valid\n\
    \t file parameters (optional)\n\
    \t\t -f <filename> -> input file (if not specified - stdin)\n\
    \t\t -o <filename> -> output file (if not specified - stdout)\n\
    \t\t -m <filename> -> path to the parity check matrix (required argument for decoding)\n";
}

int main(int argc, char* argv[]){

    Args args = Args();
	// if args parsed correctly
    if(args.ParseArgs(argc, argv)){
        BMS bms = BMS(args, argv[argc - 1]);
        bms.Run();
    }
    else
        printHelp();

    return 0;
}
