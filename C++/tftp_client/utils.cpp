/**
 * ISA project - TFTP client
 * @author Timotej Ponek, xponek00
 * @file utils.cpp
 */

#include <sys/stat.h>
#include "utils.h"

/**
 * Splits string by delimiter into vector
 * adopted from https://stackoverflow.com/a/236803
 * @param s string to split
 * @param delim delimiter
 * @param result container to which result will be stored
 */
template <typename Out>
void split(const std::string &s, char delim, Out result) {
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, delim)) {
        *result++ = item;
    }
}

/**
 * Splits string by delimiter into vector
 * adopted from https://stackoverflow.com/a/236803
 * @param s string to split
 * @param delim delimiter
 * @return vector filled with strings that were extracted
 */
std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

/**
 * @brief Gets size of file
 * based on answer from SO: https://stackoverflow.com/a/6039648
 * and many others mentioning this aproach
 * @param filename name of file
 * @param filesize reference to variable where to store file size
 * @return file size as string
 */
std::string getFileSize(const char* filename, unsigned long &filesize){
    struct stat stat_buf;
    int rc = stat(filename, &stat_buf);

    filesize = rc == 0 ? stat_buf.st_size : 0;
    return std::to_string(filesize);
}

/**
 * @brief Gets size of file
 * based on answer from SO: https://stackoverflow.com/a/6039648
 * and many others mentioning this aproach
 * @param filename name of file
 * @return file size as string
 */
std::string getFileSize(const char* filename){
    struct stat stat_buf;
    int rc = stat(filename, &stat_buf);

    long long fileSize = rc == 0 ? stat_buf.st_size : -1;
    return std::to_string(fileSize);
}