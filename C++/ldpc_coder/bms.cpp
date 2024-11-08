/**
 * BMS project 1 - LDPC coder
 * @author Timotej Ponek, xponek00
 * @file bms.cpp
 */

#include "bms.h"
#include "matrixOperations.h"

#include <algorithm>
#include <random>
#include <cstring>
#include <bitset>

#include "args.h"

#define byte_size 8
#define MAX_ITER 1000

/**
 * @brief Constructor
 * @param arguments program arguments
 * @param inputEncoding last argument, which will be used as input text if
 *                  input file is not specified
 */
BMS::BMS(Args &arguments, char *inputText)
{
  args = &arguments;
}

void BMS::Run()
{
  if (args->encode)
  {
    LoadInputEncoding();

    if (!args->matrixFile.empty())
    {
      this->H = LoadMatrix();
    }
    else
    {
      GenerateParityCheckMatrix();
    }
    GenerateCodingMatrix();
    EncodeMessage();
  }

  if (args->decode)
  {
    this->H = LoadMatrix();
    LoadInputDecoding();
    DecodeMessage();
  }
}

void BMS::LoadInputEncoding()
{
  inputEncoding.reserve(100);
  char c;
  while (std::cin.get(c))
  { // check if the character is printable
    if (std::isprint(c))
    { // check if the character is not whitespace
      if (!std::isspace(c))
      { 
        inputEncoding += c;
      }
    }
  }
  // We set this to be constant as this is the most
  // reliable size of matrix for now
  d_c = 1 * (sizeof(char) * byte_size);
  n_code = 2 * d_c;
  d_v = d_c - 1;
}

void BMS::LoadInputDecoding()
{
  char c;
  u_int i = 0;
  std::vector<char> input;
  input.reserve(H[0].size());

  while (std::cin.get(c))
  {
    // check if the character is printable & not whitespace
    if (std::isprint(c) && !std::isspace(c))
    {
      if (c == '1')
      {
        input.push_back(1);
      }
      else
      {
        input.push_back(0);
      }
      i++;
      if (i == H[0].size())
      {
        inputDecoding.push_back(input);
        i = 0;
        input.clear();
        input.reserve(H[0].size());
      }
    }
  }
}

void BMS::GenerateParityCheckMatrix()
{
  u_int n_equations = (n_code * d_v) / d_c;
  u_int block_size = n_equations / d_v;

  std::vector<std::vector<char>> block(block_size, std::vector<char>(n_code, 0));
  std::vector<std::vector<char>> H(n_equations, std::vector<char>(n_code, 0));

  // Filling the first block with consecutive ones in each row of the block
  for (u_int i = 0; i < block_size; i++)
  {
    for (u_int j = i * d_c; j < (i + 1) * d_c; j++)
    {
      block[i][j] = 1;
    }
  }
  std::copy(block.begin(), block.end(), H.begin());

  // Create remaining blocks by permutations of the first block's columns
  for (u_int i = 1; i < d_v; i++)
  {
    permuteColumns(block);
    std::copy(block.begin(), block.end(), H.begin() + i * block_size);
  }

  this->H = H;
}

void BMS::GenerateCodingMatrix()
{
  u_int n_equations = H.size();
  u_int n_code = H[0].size();

  // generate identity matrix
  std::vector<std::vector<char>> P1(n_code, std::vector<char>(n_code, 0));
  for (u_int i = 0; i < n_code; i++)
  {
    P1[i][i] = 1;
  }

  // gausJordanElimination to create row reduced H
  std::vector<std::vector<char>> HRowReduced = gaussJordanElimination(H);

  u_int n_bits = n_code - std::count_if(HRowReduced.begin(), HRowReduced.end(),
                                        [](const std::vector<char> &row)
                                        {
                                          return std::any_of(row.begin(), row.end(),
                                                             [](int val)
                                                             { return val != 0; });
                                        });

  while (true)
  {
    std::vector<int> zeros;

    for (u_int i = 0; i < std::min(n_equations, n_code); i++)
    {
      if (!HRowReduced[i][i])
      {
        zeros.push_back(i);
      }
    }

    if (!zeros.empty())
    {
      int indice_colonne_a = *std::min_element(zeros.begin(), zeros.end());

      std::vector<int> list_ones;
      for (u_int j = indice_colonne_a + 1; j < n_code; j++)
      {
        if (HRowReduced[indice_colonne_a][j])
        {
          list_ones.push_back(j);
        }
      }
      if (!list_ones.empty())
      {
        int indice_colonne_b = *std::min_element(list_ones.begin(), list_ones.end());
        // Swap columns
        for (u_int i = 0; i < n_equations; i++)
        {
          int temp = HRowReduced[i][indice_colonne_a];
          HRowReduced[i][indice_colonne_a] = HRowReduced[i][indice_colonne_b];
          HRowReduced[i][indice_colonne_b] = temp;
        }

        // Swap P1 columns
        for (u_int i = 0; i < n_code; i++)
        {
          int temp = P1[i][indice_colonne_a];
          P1[i][indice_colonne_a] = P1[i][indice_colonne_b];
          P1[i][indice_colonne_b] = temp;
        }
      }
      else
      {
        break;
      }
    }
    else
    {
      break;
    }
  }

  std::vector<std::vector<char>> P1_transposed = transposeMatrix(P1);

  // identity
  std::vector<char> identity(n_code);
  std::iota(identity.begin(), identity.end(), 0);

  // sigma
  std::vector<char> sigma(identity.begin() + n_code - n_bits, identity.end());
  sigma.insert(sigma.end(), identity.begin(),
               identity.begin() + n_code - n_bits);

  // P2 matrix
  std::vector<std::vector<char>> P2(n_code, std::vector<char>(n_code, 0));
  for (u_int i = 0; i < n_code; i++)
  {
    P2[i][sigma[i]] = 1;
  }

  std::vector<std::vector<char>> P = matrixXmatrixMultiplication(P2, P1_transposed);
  std::vector<std::vector<char>> H_new = matrixXmatrixMultiplication(H, transposeMatrix(P));

  std::vector<std::vector<char>> G_systematic(n_bits,
                                              std::vector<char>(n_code, 0));

  // create identity side of G
  for (u_int i = 0; i < n_bits; i++)
  {
    G_systematic[i][i] = 1;
  }

  // create side of G based on HRowReduced
  for (u_int i = 0; i < n_code - n_bits; i++)
  {
    for (u_int j = 0; j < n_bits; j++)
    {
      G_systematic[j][i + n_bits] = HRowReduced[i][j + n_code - n_bits];
    }
  }

  this->G = transposeMatrix(G_systematic);
  this->H = H_new;

  SaveMatrix(this->H, "matica.csv");
}

void BMS::EncodeMessage()
{
  std::string::iterator it;

  std::vector<char> inputBits;
  inputBits.reserve(d_c);
  std::vector<std::vector<u_char>> G_char = convertMatrixToBinary(this->G);

  u_int codingSize = this->H[0].size() / 2;
  u_int counter = 0;

  for (it = inputEncoding.begin(); it != inputEncoding.end(); it++)
  {
    inputBits.emplace_back(*it);
    std::bitset<8> cInBinary(*it);
    std::cout << cInBinary;
    counter += sizeof(char) * byte_size;

    // compute parity check side, reset counter and
    // clear inputBits vector for next word
    if (counter == codingSize)
    {
      for (u_int i = (1 * codingSize); i < G.size(); i++)
      {
        u_int sum = 0;
        for (u_int j = 0; j < codingSize / byte_size; j++)
        {
          sum += multiplyBits(inputBits[j], G_char[i][j]);
        }
        std::cout << sum % 2;
      }
      inputBits.clear();
      counter = 0;
    }
  }
}

std::vector<std::vector<char>> BMS::LoadMatrix()
{
  std::ifstream file;
  file.open(args->matrixFile);
  std::vector<std::vector<char>> H_matrix;

  if (file.is_open())
  {
    std::string line;
    while (std::getline(file, line))
    {
      std::vector<char> row;
      size_t pos = 0;
      std::string token;

      while ((pos = line.find(',')) != std::string::npos)
      {
        token = line.substr(0, pos);
        row.push_back(std::stoi(token));
        line.erase(0, pos + 1);
      }
      row.push_back(std::stoi(line));
      H_matrix.push_back(row);
    }

    file.close();
  }
  else
  {
    std::cerr << "Unable to open file: " << args->matrixFile << std::endl;
    exit(1);
  }

  return H_matrix;
}

void BMS::SaveMatrix(const std::vector<std::vector<char>> &matrix, const std::string &filename)
{
  std::ofstream file(filename);

  if (!file.is_open())
  {
    std::cerr << "Error opening file: " << filename << std::endl;
    return;
  }

  for (const auto &row : matrix)
  {
    for (u_int i = 0; i < row.size(); i++)
    {
      file << +row[i];
      if (i != row.size() - 1)
      {
        file << ",";
      }
    }
    file << std::endl;
  }

  file.close();
}

std::vector<Edge> BMS::CreateTannerGraph(const std::vector<std::vector<char>> &parityCheckMatrix)
{
  std::vector<Edge> edges;

  u_int numVariableNodes = parityCheckMatrix[0].size();
  u_int numCheckNodes = parityCheckMatrix.size();

  for (u_int i = 0; i < numCheckNodes; i++)
  {
    for (u_int j = 0; j < numVariableNodes; j++)
    {
      if (parityCheckMatrix[i][j] == 1)
      {
        edges.push_back({j, i});
      }
    }
  }
  return edges;
}

std::vector<char> BMS::ldpcDecodingWBF(const std::vector<Edge> &edges, const std::vector<char> &receivedBits)
{
  // Initialize the variable nodes with the received bits
  std::vector<char> variableNodes(receivedBits);

  // Perform WBF decoding iterations
  for (u_int iteration = 0; iteration < MAX_ITER; iteration++)
  {
    // Update the check nodes
    for (const Edge &edge : edges)
    {
      u_int checkNode = edge.checkNode;
      int sum = 0;
      for (const Edge &connectedEdge : edges)
      {
        if (connectedEdge.checkNode == checkNode && connectedEdge.variableNode != edge.variableNode)
        {
          sum += variableNodes[connectedEdge.variableNode];
        }
      }
      variableNodes[edge.variableNode] = sum % 2;
    }

    // Check for convergence
    bool converged = true;
    for (const Edge &edge : edges)
    {
      u_int checkNode = edge.checkNode;
      int sum = 0;
      for (const Edge &connectedEdge : edges)
      {
        if (connectedEdge.checkNode == checkNode && connectedEdge.variableNode != edge.variableNode)
        {
          sum += variableNodes[connectedEdge.variableNode];
        }
      }
      if (sum % 2 != receivedBits[edge.variableNode])
      {
        converged = false;
        break;
      }
    }

    // If converged, stop decoding
    if (converged)
    {
      break;
    }
  }

  return variableNodes;
}

/**
 * @brief Function to decode input in cycles
 * (based on the size of parity check matrix H)
 */
void BMS::DecodeMessage()
{

  for (auto inputPiece : inputDecoding)
  {

    bool decode_again = true;

    // First find out, if recieved vector is without error
    if (isCodeword(inputPiece, this->H) &&
        isValidDecoded(inputPiece))
    {
      decode_again = false;
    }

    if (!decode_again)
    {
      // Syndrom was 0 during initial decode, printing
      printDecoded(inputPiece);
    }
    else
    {
      // Syndrom was 1 during initial decode, trying to correct
      // errors using WBF
      std::vector<Edge> tannerGraph = CreateTannerGraph(this->H);

      std::vector<char> inputDecoded = ldpcDecodingWBF(tannerGraph, inputPiece);

      if (isCodeword(inputDecoded, this->H) &&
          isValidDecoded(inputDecoded))
      {
        printDecoded(inputDecoded);
      }
    }
  }
}
