/**
 * KRY project 1 - Affine cipher
 * @author Timotej Ponek, xponek00
 * @file kry.cpp
 */

#include "kry.h"
#include "args.h"
#include <algorithm>

/**
 * @brief Constructor
 * @param arguments program arguments
 * @param inputText last argument, which will be used as input text if
 *                  input file is not specified
 */
Kry::Kry(Args &arguments, char* inputText){

    args = &arguments;
    this->inputText = inputText;

    //redirect stdout to file
    if (!args->outputFile.empty() && !args->freqAnalysis) {
        freopen(args->outputFile.c_str(), "w", stdout);
    }

    a = args->a;
    b = args->b;
}


/// @brief Main method, runs corresponding mode based on arguments
void Kry::Run(){
    if (args->encrypt) {
        if (!args->inputFile.empty()) {
            EncryptText();
        }
        else {
            EncryptText(inputText);
        }
    }
    else if (args->decrypt) {
        mulInv = CalcMulInv(args->a, Az_lenght);

        if (!args->inputFile.empty()) {
            DecryptText();
        }
        else {
            DecryptText(inputText);
        }
    }
    else if (args->freqAnalysis) {
        
        if (!args->inputFile.empty()) {
            CalculateFrequencies();
        }
        else {
            CalculateFrequencies(inputText);
        }

        //sort calculated values
        std::vector<std::pair<char, unsigned>> onegrams = SortByFrequencies(charCount);
        std::vector<std::pair<std::string, unsigned>> bigrams = SortByFrequencies(bigramCount);
        std::vector<std::pair<std::string, unsigned>> trigrams = SortByFrequencies(trigramCount);

        std::pair<int, int> derivedParams = DeriveParameters(
                    onegrams,
                    bigrams,
                    trigrams);

        SetParameters(derivedParams.first, derivedParams.second);

        //output params
        std::cout << "a=" << derivedParams.first <<
        ",b=" << derivedParams.second << std::endl;

        //redirect stdout to file
        if (!args->outputFile.empty()) {
            freopen(args->outputFile.c_str(), "w", stdout);
        }

        if (!args->inputFile.empty()) {
            DecryptText();
        }
        else {
            DecryptText(inputText);
        }
    }
}

/// @brief Set kes for encryption/decryption
/// @param a 
/// @param b 
void Kry::SetParameters(int a, int b){
    this->a = a;
    this->b = b;
}

/////////// Encrypt //////////

/// @brief input is file
void Kry::EncryptText(){
    input.open(args->inputFile);    
    char c;
    while(input >> std::noskipws >> c){
        EncryptChar(toupper(c));
    }
    input.close();
}

/// @brief input is string
/// @param input input string
void Kry::EncryptText(std::string input){
    for (char& c : input) {
        EncryptChar(toupper(c));
    }
}

/// @brief encrypts given character
/// @param x 
void Kry::EncryptChar(int x){
    if (isspace(x)) {
        std::cout << char(x);
    }
    else {
        int eX = (a * (x-65) + b) % Az_lenght + 65;
        std::cout << char(eX);
    }
}

/////////// Decrypt //////////

/// @brief Calculates modular multiplicative inverse
///        adopted from https://rosettacode.org/wiki/Modular_inverse#C++
/// @param a a key
/// @param m modulo
int Kry::CalcMulInv(int a, int b)
{
    int b0 = b, t, q;
    int x0 = 0, x1 = 1;
    if (b == 1) {
        return 1;
    }
    while (a > 1) {
        q = a / b;
        t = b, b = a % b, a = t;
        t = x0, x0 = x1 - q * x0, x1 = t;
    }
    if (x1 < 0) {
        x1 += b0;
    }
    return x1;
}

/// @brief input is file
void Kry::DecryptText(){
    mulInv = CalcMulInv(a, Az_lenght);
    input.open(args->inputFile);
    char c;
    while(input >> std::noskipws >> c){
        DecryptChar(toupper(c));
    }
    input.close();
}

/// @brief input is string
/// @param input input string
void Kry::DecryptText(std::string input){
    mulInv = CalcMulInv(a, Az_lenght);
    for (char& c : input) {
        DecryptChar(toupper(c));
    }
}

/// @brief decrypts given character
/// @param x 
void Kry::DecryptChar(int x){
    if (isspace(x)) {
        std::cout << char(x);
    }
    else {
        // (value % n + n) % n -> this deals with negative values
        int dX = ( ((mulInv * ((x-65) - b)) % Az_lenght) + Az_lenght ) % Az_lenght + 65;
        std::cout << char(dX);
    }
}

/////////// FreqAnalysis //////////

/// @brief Input is file.
///        Calculates frequencies of bigrams, trigrams and characters
///        in encrypted text
void Kry::CalculateFrequencies(){
    input.open(args->inputFile);
    unsigned indexer = 0;
    char c;
    std::string word = "";
    while(input >> std::noskipws >> c){
        if (isspace(c)) {
           word = "";
           indexer = 0;
        }
        else{
            indexer++;
            word.push_back(c);
            charCount[toupper(c)] += 1;
            if (!(word.length() < 2)) {
                if (word.length() > 3) {
                    trigramCount[word.substr(indexer - 3)] += 1;
                }
                bigramCount[word.substr(indexer - 2)] += 1;
            }
        }
    }
    input.close();
}

/// @brief Input is string.
///        Calculates frequencies of bigrams, trigrams and characters
///        in encrypted text
/// @param input input string
void Kry::CalculateFrequencies(std::string input){
    unsigned indexer = 0;
    std::string word = "";
    for (const char& c : input){
        if (isspace(c)) {
           word = "";
           indexer = 0;
        }
        else{
            indexer++;
            word.push_back(c);
            charCount[toupper(c)] += 1;
            if (!(word.length() < 2)) {
                if (word.length() > 3) {
                    trigramCount[word.substr(indexer - 3)] += 1;
                }
                bigramCount[word.substr(indexer - 2)] += 1;
            }
        }
    }
}

/// @brief sorts given map in descending order based on the count of occurances
/// @param toSort map to sort
/// @return vector sorted in descending order
std::vector<std::pair<char, unsigned>> Kry::SortByFrequencies(
    std::map<char, unsigned> toSort 
){
    std::vector<std::pair<char, unsigned>> tmp;
    for (const auto& [c, count] : toSort) {
        tmp.emplace_back(c, count);
    }
    sort(tmp.begin(), tmp.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });
    return tmp;
}

/// @brief sorts given map in descending order based on the count of occurances
/// @param toSort map to sort
/// @return vector sorted in descending order
std::vector<std::pair<std::string, unsigned>> Kry::SortByFrequencies(
    std::map<std::string, unsigned> toSort
){
    std::vector<std::pair<std::string, unsigned>> tmp;
    for (const auto& [c, count] : toSort) {
        tmp.emplace_back(c, count);
    }
    sort(tmp.begin(), tmp.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });
    return tmp;
}


/// @brief 
/// @param a 
/// @param b 
/// @param encrTrigrams most frequent trigrams in encrypted text
/// @return number of succesfully decrypted trigrams to most frequent trigrams in czech language
int Kry::TryTrigrams(int a, int b, std::vector<std::pair<std::string, unsigned>> encrTrigrams){
    int count = 0;
    
    for (size_t i = 0; i < encrTrigrams.size(); i++) {
        std::string currTrigram = encrTrigrams[i].first;
        for (const auto& c : mostCommonTrigrams) {
            if(
                ((mulInv * (currTrigram[0] - 65 - b) % Az_lenght) + Az_lenght) % Az_lenght == c[0] - 65 &&
                ((mulInv * (currTrigram[1] - 65 - b) % Az_lenght) + Az_lenght) % Az_lenght == c[1] - 65 &&
                ((mulInv * (currTrigram[2] - 65 - b) % Az_lenght) + Az_lenght) % Az_lenght == c[2] - 65
            ){
                count++;
            }
        }
    }
    return count;
}


/// @brief 
/// @param a 
/// @param b 
/// @param encrBigrams most frequent trigrams in encrypted text
/// @return number of succesfully decrypted bigrams to most frequent bigrams in czech language
int Kry::TryBigrams(int a, int b, std::vector<std::pair<std::string, unsigned>> encrBigrams){
    int count = 0;
    
    for (size_t i = 0; i < encrBigrams.size(); i++) {
        std::string currBigram = encrBigrams[i].first;
        for (const auto& c : mostCommonBigrams) {
            if(
                ((mulInv * (currBigram[0] - 65 - b) % Az_lenght) + Az_lenght) % Az_lenght == c[0] - 65 &&
                ((mulInv * (currBigram[1] - 65 - b) % Az_lenght) + Az_lenght) % Az_lenght == c[1] - 65
            ){
                count++;
                break;
            }
        }
    }
    return count;
}

/// @brief 
/// @param encrOnegrams count of literals in encrypted text
/// @param encrBigrams count of bigrams in encrypted text
/// @param encrTrigrams count of trigrams in encrypted text
/// @return pair of params <a, b>, for which most bigrams and trigrams
///         were succesfully decrypted to most frequent czech bigrams
///         and trigrams
std::pair<int, int> Kry::DeriveParameters(
        std::vector<std::pair<char, unsigned>> encrOnegrams,
        std::vector<std::pair<std::string, unsigned>> encrBigrams,
        std::vector<std::pair<std::string, unsigned>> encrTrigrams
    ){
        
    int aValues[12] = {1, 3, 5, 7, 9, 11, 15, 17, 19, 21, 23, 25};

    // counter of correctly decoded, <a, b>
    std::vector<std::pair<int, std::pair<int, int>>> derivedParams;

    // resize only if there are too much trigrams
    // needed becazse resize inserts unnecessary empty ("") values
    // if current size is smaller
    if(encrTrigrams.size() > 40){
        encrTrigrams.resize(30);
        encrBigrams.resize(30);
    }

    for (int a : aValues) {
        for (int b=0; b < 26; b++) {
            mulInv = CalcMulInv(a, Az_lenght);
            char decX = ((mulInv * (encrOnegrams[0].first - 65 - b) % Az_lenght) + Az_lenght) % Az_lenght + 65;
            if (mostCommonLiterals.find(decX) != mostCommonLiterals.end()) {
                int correct3 = TryTrigrams(a, b, encrTrigrams);
                int correct2 = TryBigrams(a, b, encrBigrams);
                derivedParams.emplace_back(correct2+correct3, std::make_pair(a, b));
            }
        }
    }

    sort(derivedParams.begin(), derivedParams.end(), [](const auto& a, const auto& b) {
        return a.first > b.first;
    });

    return derivedParams[0].second;
}