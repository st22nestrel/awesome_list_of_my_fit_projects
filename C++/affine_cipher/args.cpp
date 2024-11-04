/**
 * KRY project 1 - Affine cipher
 * @author Timotej Ponek, xponek00
 * @file args.cpp
 */

#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include "args.h"

Args::Args(){
}

/**
 * @brief Parses arguments
 * @param argc 
 * @param argv
 * @return true if all needed arguments were set, false otherwise
 */
bool Args::ParseArgs(int argc, char* argv[])
{
    int c;

    while ((c = getopt_long(argc, argv, "edcf:o:a:b:", NULL, NULL)) != -1)
    switch(c){
        case 'e':
            if(!decrypt && !freqAnalysis)
                encrypt = true;
            else{
                fprintf(stderr, "Cannot launch in multiple modes, select one mode only."
                                " Available modes are '-e' '-d' '-c'\n");
                return false;
            }
        break;

        case 'd':
            if(!encrypt && !freqAnalysis)
                decrypt = true;
            else{
                fprintf(stderr, "Cannot launch in multiple modes, select one mode only."
                                " Available modes are '-e' '-d' '-c'\n");
                return false;
            }
        break;

        case 'c':
            if(!encrypt && !decrypt)
                freqAnalysis = true;
            else{
                fprintf(stderr, "Cannot launch in multiple modes, select one mode only."
                                " Available modes are '-e' '-d' '-c'\n");
                return false;
            }
        break;

        // optional
        case 'f':
            if(optarg == NULL){
                fprintf(stderr, "Input file missing");
                return false;
            }
            else{
                Args::inputFile = optarg;
            }
        break;

        // optional
        case 'o':
            if(optarg == NULL){
                fprintf(stderr, "Output file missing");
                return false;
            }
            else{
                Args::outputFile = optarg;
            }
        break;

        case 'a':
        {
            int a = std::stoi(optarg);
            if(a < 1 || a > 26){
                fprintf(stderr, "Invalid a key");
                return false;
            }
            else{
                Args::a = a;
            }
        }
        break;

        case 'b':
        {
            int b = std::stoi(optarg);
            if(b < 1 || b > 26){
                fprintf(stderr, "Invalid a key");
                return false;
            }
            else{
                Args::b = b;
            }
        }
        break;
        // ignore undefined args
        default:
            continue;
    }
    
    // check if no mode was selected
    if(!(Args::encrypt || Args::decrypt || Args::freqAnalysis))
        return false;

    return true;
}