# Affine cipher cracker

## About

This project implements an Affine cipher tool that can:
- Encrypt text using provided keys
- Decrypt text using provided keys
- Crack encrypted text without keys using frequency analysis

### School subject and project

- *subject*: **KRY - Cryptography**
- *project*: Project 1, Affine cipher (Projekt 1, Afinitní šifra)

## Usage

### Prerequisites

- A C++ compiler (e.g., g++)


### How to run

1. Compile the project
    ```bash
    make
    ```

2. Run the binary with appropriate arguments:
    - for encryption:
        ```bash
        ./kry -e -a <key_a> -b <key_b>
        ```
    - for decryption with known keys: 
        ```bash
        ./kry -d -a <key_a> -b <key_b>
        ```
    - for cracking encrypted text (without keys):
        ```bash
        ./kry -c
        ```


#### Command Line Arguments

> [!NOTE]
> Command line arguments are also displayed in shell when running `./kry`

Mode specification (required):

- `-e` : encrypt
- `-d` : decrypt
- `-c` : decrypt without keys (using frequency analysis)

Keys (required for -e and -d modes):

- `-a <val>` : value of 'a' part of key
- `-b <val>` : value of 'b' part of key

File parameters (optional):

- `-f <filename>` : input file (if not specified, uses stdin)
- `-o <filename>` : output file (if not specified, uses stdout)

### Examples

1. Encrypt text from argument:
```bash
./kry -e -a 5 -b 8 "HELLO WORLD"
```

2. Decrypt text using known keys:
```bash
./kry -d -a 5 -b 8 "RCLLA OAPLX"

```

3. Crack encrypted text without keys:
```bash
./kry -c -f encrypted.txt
```

### Input format

The input should contain:

- Plain text for encryption
- Encrypted text for decryption/cracking
- Text should contain only letters A-Z (will be converted to uppercase)

Note that the cracking part is designed to use frequency analysis for Czech language, so it might not crack encrypted text in English. It also won't work if the cracked text is too short.

### Output

The program will output:

- For encryption: the encrypted text
- For decryption: the decrypted text
- For cracking: the decrypted text and the discovered key parameters (a and b)

## Misc 
- Most frequent bigrams and trigrams in czech were selected from this resource - https://nlp.fi.muni.cz/cs/FrekvenceSlovLemmat
- Most frequent literals in czech were selected based on this resource - http://sas.ujc.cas.cz/archiv.php?art=2913