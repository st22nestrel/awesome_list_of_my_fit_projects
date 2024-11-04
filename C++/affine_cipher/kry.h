/**
 * KRY project 1 - Affine cipher
 * @author Timotej Ponek, xponek00
 * @file kry.h
 */

#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <vector>
#include <array>

/// lenght of alphabet
#define Az_lenght 26

class Args;

/// @brief Main class of this project
class Kry
{
private:
    Args* args;
    char* inputText;

    int a;
    int b;

    // used only if '-f' or '-o' arguments are specified
    std::ifstream input;
    std::ofstream output;

    int mulInv;

    std::map<char, unsigned> charCount = {
        {'A', 0}, {'B', 0}, {'C', 0}, {'D', 0}, {'E', 0},
        {'F', 0}, {'G', 0}, {'H', 0}, {'I', 0}, {'J', 0},
        {'K', 0}, {'L', 0}, {'M', 0}, {'N', 0}, {'O', 0},
        {'P', 0}, {'Q', 0}, {'R', 0}, {'S', 0}, {'T', 0},
        {'U', 0}, {'V', 0}, {'W', 0}, {'X', 0}, {'Y', 0},
        {'Z', 0}
    };

    std::map<std::string, unsigned> bigramCount;
    std::map<std::string, unsigned> trigramCount;

    //these literals were chosen based on research on this website
    //http://sas.ujc.cas.cz/archiv.php?art=2913
    const std::set<char> mostCommonLiterals = {'A', 'E', 'O', 'R', 'I', 'N'};
    
    //from website https://nlp.fi.muni.cz/cs/FrekvenceSlovLemmat
    const std::array<std::string, 40> mostCommonBigrams = {{
        "ST", "NI", "PO", "OV", "RO", "EN", 
        "NA", "JE", "PR", "TE", "LE", "KO", 
        "NE", "OD", "RA", "TO", "OU", "NO", 
        "LA", "LI", "HO", "DO", "OS", "SE", 
        "TA", "AL", "ED", "AN", "CE", "VA", 
        "PR", "AT", "RE", "ER", "TI", "EM", 
        "IN", "SK", "LO" }};

    const std::array<std::string, 40> mostCommonTrigrams = {{
        "PRO", "OST", "STA", "PRE", "TER", "ENI", 
        "OVA", "POD", "KTE", "PRA", "EHO", "STI", 
        "RED", "KON", "NOS", "ICK", "OVA", "PRI", 
        "SOU", "IST", "EDN", "SKE", "PRI", "ODN", 
        "TEL", "ANI", "ENT", "STR", "OVE", "NOV", 
        "POL", "SPO", "VAT", "NIM", "JAK", "VAL", 
        "DNI", "STO", "TAK", "LOV" }};
    //----------------------------------------------------------

    int CalcMulInv(int a, int b);
    void SetParameters(int a, int b);

    void DecryptText();
    void DecryptText(std::string input);
    void DecryptChar(int x);

    void CalculateFrequencies();
    void CalculateFrequencies(std::string input);

    std::vector<std::pair<char, unsigned>> SortByFrequencies(std::map<char, unsigned> toSort);
    std::vector<std::pair<std::string, unsigned>> SortByFrequencies(std::map<std::string, unsigned> toSort);


    int TryTrigrams(int a, int b, std::vector<std::pair<std::string, unsigned>> plainTrigrams);
    int TryBigrams(int a, int b, std::vector<std::pair<std::string, unsigned>> plainBigrams);

    std::pair<int, int> DeriveParameters(
        std::vector<std::pair<char, unsigned>> plainOnegrams,
        std::vector<std::pair<std::string, unsigned>> plainBigrams,
        std::vector<std::pair<std::string, unsigned>> plainTrigrams);

    void EncryptText();
    void EncryptText(std::string input);
    void EncryptChar(int c);

public:
    Kry(Args &arguments, char* inputText);

    void Run();
};