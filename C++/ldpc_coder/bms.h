/**
 * BMS project 1 - LDPC coder
 * @author Timotej Ponek, xponek00
 * @file bms.h
 */

#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <vector>

/// lenght of alphabet
#define Az_lenght 26
#define MAX_ITER 1000

class Args;

/// @brief Structure to represent Tanner graph edges
struct Edge
{
    u_int variableNode; // Index of the variable node
    u_int checkNode;    // Index of the check node
};

/// @brief Main class of this project
class BMS
{
private:
    Args *args;
    std::string inputEncoding;
    std::vector<std::vector<char>> inputDecoding;

    std::vector<std::vector<char>> H;
    std::vector<std::vector<char>> H_T;
    std::vector<std::vector<char>> G;

    u_int n_code = 0;
    u_int d_c = 0;
    u_int d_v = 0;

    /**
     * @brief Loads input for encoding phase of LDPC
     */
    void LoadInputEncoding();
    /**
     * @brief Loads input for decoding phase of LDPC
     */
    void LoadInputDecoding();
    /**
     * @brief Generates Parity Check Matrix H based on internal parameters
     * d_c, d_v, n_code
     */
    void GenerateParityCheckMatrix();
    /**
     * @brief Generates Generator Matrix G based on
     * Parity Check Matrix H
     */
    void GenerateCodingMatrix();

    /**
     * @brief Function encodes input message
     */
    void EncodeMessage();
    void DecodeMessage();
    std::vector<u_char> decode(const std::vector<double> &r);

    /**
     * @brief Loads matrix H from file given by args
     * @return loaded matrix H
     */
    std::vector<std::vector<char>> LoadMatrix();
    /**
     * @brief Saves matrix H to given file
     * @param matrix matrix to save
     * @param filename
     */
    void SaveMatrix(const std::vector<std::vector<char>> &matrix, const std::string &filename);

    /**
     * @brief Function to transform parity check matrix into Tanner graph
     * @param parityCheckMatrix
     * @return Tanner graph representation of parityCheckMatrix
     */
    std::vector<Edge> CreateTannerGraph(const std::vector<std::vector<char>> &parityCheckMatrix);

    /**
     * @brief Decodes recievedBits according to WBF algorithm
     * @param edges Tanner graph
     * @param recievedBits vector of input bits
     */
    std::vector<char> ldpcDecodingWBF(const std::vector<Edge> &edges, const std::vector<char> &receivedBits);

public:
    /// @brief Main method, runs corresponding mode based on arguments
    BMS(Args &arguments, char *inputText);

    void Run();
};