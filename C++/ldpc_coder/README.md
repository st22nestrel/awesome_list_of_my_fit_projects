# LDPC Coder

## About

This project implements an LDPC (Low-Density Parity-Check) coder that provides functionality to:

- Encode ASCII messages using LDPC coding
- Decode binary messages using LDPC decoding
- Generate and handle parity check matrices
- Implement Weighted Bit-Flipping (WBF) decoding algorithm
- Work with Tanner graphs for LDPC decoding

### School subject and project

-   *subject*: **BMS - Wireless and Mobile Networks**
-   *project*: Project 1, LDPC Coder

## Usage

### Prerequisites

- C++ compiler (with C++11 support)
- Make

### How to run

1. Compile the project
    ```bash
    make
    ```

2. Run the binary
    ```bash
    ./bms
    ```

Program arguments:

- `-e`: Encode mode - takes ASCII characters as input
- `-d`: Decode mode - takes binary (1s and 0s) as input
- `-f <filename>`: Specify input file (optional, uses stdin if not specified)
- `-o <filename>`: Specify output file (optional, uses stdout if not specified)
- `-m <filename>`: Specify matrix file with parity check matrix (required for `-d` decoding option, when encoding, the matrix is auto generated)

### Examples

1. Encode ASCII text from stdin:
    ```bash
    ./bms -e
    ```

2. Decode binary sequence from a file:
    ```bash
    ./bms -d -f input.txt -m matica.csv
    ```

3. Encode with custom output file:
    ```bash
    ./bms -e -o encoded.txt
    ```

### Input/Output Format

#### Encoding

- Input: ASCII characters
- Output: Binary sequence representing the encoded message

### Decoding

- Input: Binary sequence (1s and 0s)
- Output: ASCII characters representing the decoded message

> [!NOTE] 
> The program generates a parity check matrix during encoding which is saved as 'matica.csv'. This matrix must be specified with `-m` argument when decoding.

## Notes
Project uses only 16x16 (or 16x14) parity check matrix H
for encoding, because:

- it cannot create larger matrix (error in generating of generator matrix G, because created H is not always invertible)
- when dealing with variable size input, encoding of input that is not exactly coresponding to size of the matrix (i.e. length of the input is not the number of collumns of matrix H), input would be trimmed, but with usage of matrix 16x16, nothing is trimmed

> [!WARNING] 
> Decoding with matrices larger than 16x16 can be slow (like extremly slow)... code was not optimized for usage of large matrices

When provided with correct H (left side invertible and right side identity matrix), encoding should work also with larger than 16x16 matrices.