/**
 * BMS project 1 - LDPC coder
 * @author Timotej Ponek, xponek00
 * @file args.cpp
 */

#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include "args.h"

Args::Args()
{
}

/**
 * @brief Parses arguments
 * @param argc
 * @param argv
 * @return true if all needed arguments were set, false otherwise
 */
bool Args::ParseArgs(int argc, char *argv[])
{
    int c;

    while ((c = getopt_long(argc, argv, "edf:o:m:", NULL, NULL)) != -1)
        switch (c)
        {
        case 'e':
            if (!decode)
                encode = true;
            else
            {
                fprintf(stderr, "Cannot launch in multiple modes, select one mode only."
                                " Available modes are '-e' '-d' '-c'\n");
                return false;
            }
            break;

        case 'd':
            if (!encode)
                decode = true;
            else
            {
                fprintf(stderr, "Cannot launch in multiple modes, select one mode only."
                                " Available modes are '-e' '-d' '-c'\n");
                return false;
            }
            break;

        case 'm':
            if (optarg == NULL)
            {
                fprintf(stderr, "Matrix file missing");
                return false;
            }
            else
            {
                Args::matrixFile = optarg;
            }
            break;

        // ignore undefined args
        default:
            continue;
        }

    // check if no mode was selected
    if (!(Args::encode || Args::decode))
        return false;

    return true;
}