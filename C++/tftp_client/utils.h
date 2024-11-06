/**
 * ISA project - TFTP client
 * @author Timotej Ponek, xponek00
 * @file utils.h
 */
#include <string>
#include <vector>
#include <sstream>

template <typename Out>
void split(const std::string &s, char delim, Out result);

std::vector<std::string> split(const std::string &s, char delim);

std::string getFileSize(const char* filename, unsigned long &filesize);

std::string getFileSize(const char* filename);