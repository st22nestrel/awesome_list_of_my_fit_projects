/**
 * BMS project 1 - LDPC coder
 * @author Timotej Ponek, xponek00
 * @file matrixOperations.h
 */

#include <vector>
#include <random>

/**
 * @brief Function to create permutations of columns
 * @param matrix
 * @return matrix with permuted columns
 */
void permuteColumns(std::vector<std::vector<char>> &matrix);
/**
 * @brief Function to compute the binary row reduced echelon form of a matrix
 * @param X matrix to perfom gaussJordanElimination on
 * @return row reduced echolon form of input matrix X
 */
std::vector<std::vector<char>> gaussJordanElimination(
    const std::vector<std::vector<char>> &X);
/**
 * @brief Function to transpose a matrix
 * @param matrix
 * @return matrix transposed
 */
std::vector<std::vector<char>> transposeMatrix(
    const std::vector<std::vector<char>> &matrix);

/**
 * @brief multiplies input params bitwise
 * @param a
 * @param b
 */
int multiplyBits(char a, char b);

/**
 * @brief Converts matrix to space effiect form
 * -> 8 * '0'or'1' are saved to one char
 * @param matrix to convert
 * @return matrix that takes less space
 */
std::vector<std::vector<u_char>> convertMatrixToBinary(
    const std::vector<std::vector<char>> &matrix);

/**
 * @brief Function performs matrix by matrix multiplication
 * @param matrix1
 * @param matrix2
 * @return result of matrix1*matrix2
 */
std::vector<std::vector<char>> matrixXmatrixMultiplication(
    const std::vector<std::vector<char>> &matrix1,
    const std::vector<std::vector<char>> &matrix2);

/**
 * @brief Function performs vector by matrix multiplication
 * @param vector
 * @param matrix
 * @return result of vector*matrix
 */
std::vector<char> vectorXmatrixMultiplication(
    const std::vector<char> &vector,
    const std::vector<std::vector<char>> &matrix);

/**
 * @brief Checks if input vector is valid codeword, which means
 * all parity checks will pass
 * @param v input vector to check
 * @param H parity check matrix
 * @return true if is valid codeword, false otherwise
 */
bool isCodeword(const std::vector<char> &v, const std::vector<std::vector<char>> &H);

/**
 * @brief Checks if the obtained syndrome vector contains any '1'
 * @param vector syndrome vector
 * @return false if vector contains all '0', true otherwise
 */
bool hasSyndrome(const std::vector<u_char> &vector);
/**
 * @brief Checks if the input vector of '0'&'1' can be converted
 * to printable and non space character
 * @param v input vector
 * @return true if vector can be converted to printable and
 * non space character, false otherwise
 */
bool isValidDecoded(const std::vector<char> &v);
/**
 * @brief Prints vector of '0'&'1' as chars
 * @param v input vector
 */
void printDecoded(const std::vector<char> &v);