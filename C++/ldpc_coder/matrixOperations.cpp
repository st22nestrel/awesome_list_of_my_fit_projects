/**
 * BMS project 1 - LDPC coder
 * @author Timotej Ponek, xponek00
 * @file matrixOperations.cpp
 */

#include <algorithm>
#include <iostream>
#include <string>
#include <bitset>
#include "matrixOperations.h"

#define byte_size 8

void permuteColumns(std::vector<std::vector<char>> &matrix)
{
  for (auto &row : matrix)
  {
    std::shuffle(row.begin(), row.end(), std::mt19937(0));
  }
}

std::vector<std::vector<char>> gaussJordanElimination(
    const std::vector<std::vector<char>> &X)
{
  std::vector<std::vector<char>> A = X;
  int m = A.size();
  int n = A[0].size();

  std::vector<std::vector<char>> P; // Transformation matrix

  int pivot_old = -1;
  for (int j = 0; j < n; j++)
  {
    std::vector<char> filtre_down;
    for (int i = pivot_old + 1; i < m; i++)
    {
      filtre_down.push_back(A[i][j]);
    }
    int pivot = std::distance(
                    filtre_down.begin(),
                    std::max_element(filtre_down.begin(), filtre_down.end())) +
                pivot_old + 1;

    if (A[pivot][j])
    {
      pivot_old++;
      if (pivot_old != pivot)
      {
        std::swap(A[pivot], A[pivot_old]);
      }

      for (int i = 0; i < m; i++)
      {
        if (i != pivot_old && A[i][j])
        {
          for (int k = 0; k < n; k++)
          {
            A[i][k] = abs(A[i][k] - A[pivot_old][k]);
          }
        }
      }
    }

    if (pivot_old == m - 1)
    {
      break;
    }
  }

  return A;
}

std::vector<std::vector<char>> transposeMatrix(
    const std::vector<std::vector<char>> &matrix)
{
  int rows = matrix.size();
  int cols = matrix[0].size();

  std::vector<std::vector<char>> transposed(cols, std::vector<char>(rows, 0));

  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      transposed[j][i] = matrix[i][j];
    }
  }

  return transposed;
}

std::vector<std::vector<u_char>> convertMatrixToBinary(
    const std::vector<std::vector<char>> &matrix)
{
  u_char c = 0;
  std::vector<std::vector<u_char>> charMatrix;

  for (const std::vector<char> &row : matrix)
  {
    std::vector<u_char> charRow;
    char counter = 0;

    for (int num : row)
    {
      counter++;
      if (num)
      {
        c = (c | 1);
      }
      else
      {
        c = (c | 0);
      }
      if (counter == 8)
      {
        charRow.push_back(c);
        counter = 0;
        c = 0;
      }
      c = c << 1;
    }
    charMatrix.push_back(charRow);
  }
  return charMatrix;
}

std::vector<std::vector<char>> matrixXmatrixMultiplication(
    const std::vector<std::vector<char>> &matrix1,
    const std::vector<std::vector<char>> &matrix2)
{
  u_int m = matrix1.size();
  u_int n = matrix1[0].size();
  u_int p = matrix2.size();
  u_int q = matrix2[0].size();

  if (n != p)
  {
    throw std::invalid_argument(
        "Matrices are not compatible for multiplication.");
  }

  std::vector<std::vector<char>> result(m, std::vector<char>(q, 0));

  for (u_int i = 0; i < m; i++)
  {
    for (u_int j = 0; j < q; j++)
    {
      for (u_int k = 0; k < n; k++)
      {
        result[i][j] += matrix1[i][k] * matrix2[k][j];
      }
    }
  }

  return result;
}

bool hasSyndrome(const std::vector<char> &vector)
{
  for (char val : vector)
  {
    if (val)
    {
      return true;
    }
  }
  return false;
}

int multiplyBits(char a, char b)
{
  u_char result = a & b;
  int count = 0;

  for (int i = 0; i < 8; i++)
  {
    if ((result & (1 << i)) != 0)
    {
      count++;
    }
  }
  return count;
}

std::vector<char> vectorXmatrixMultiplication(
    const std::vector<char> &vector,
    const std::vector<std::vector<char>> &matrix)
{
  std::vector<char> result;
  for (u_int i = 0; i < matrix.size(); i++)
  {
    u_char sum = 0;
    for (u_int j = 0; j < vector.size(); j++)
    {
      sum += vector[j] * matrix[i][j];
    }
    sum = sum % 2;
    result.emplace_back(sum);
  }
  return result;
}

bool isCodeword(const std::vector<char> &v, const std::vector<std::vector<char>> &H)
{
  return !hasSyndrome(vectorXmatrixMultiplication(v, H));
}

bool isValidDecoded(const std::vector<char> &v)
{
  u_char counter = 0;
  u_char c_v = 0;
  for (u_int i = 0; i < v.size() / 2; i++)
  {
    counter++;
    if (v[i] == 1)
    {
      c_v = (c_v | 1);
    }
    else
    {
      c_v = (c_v | 0);
    }
    if (counter == 8)
    {
      if (!isprint(c_v) || isspace(c_v))
      {
        return false;
      }
      counter = 0;
      c_v = 0;
    }
    c_v = c_v << 1;
  }
  return true; // is valid decoded
}

void printMatrix(const std::vector<std::vector<char>> &matrix)
{
  for (u_int i = 0; i < matrix.size(); i++)
  {
    for (u_int j = 0; j < matrix[i].size(); j++)
    {
      std::cout << +matrix[i][j] << ", ";
    }
    std::cout << std::endl;
  }
}

void printDecoded(const std::vector<char> &v)
{
  u_char counter = 0;
  u_char c_v = 0;
  for (u_int i = 0; i < v.size() / 2; i++)
  {
    counter++;
    if (v[i] == 1)
    {
      c_v = (c_v | 1);
    }
    else
    {
      c_v = (c_v | 0);
    }
    if (counter == 8)
    {
      std::cout << c_v;
      counter = 0;
      c_v = 0;
    }
    c_v = c_v << 1;
  }
}