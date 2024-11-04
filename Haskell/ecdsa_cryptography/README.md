# ECDSA cryptography

## About

This project is an ECDSA (Elliptic Curve Digital Signature Algorithm) implementation that provides functionality to:

- Generate key pairs (private and public keys) for ECDSA
- Signs messages using ECDSA
- Verify ECDSA signatures
- Handles elliptic curve operations (point addition, doubling, multiplication)

### School subject and project

-   *subject*: **FLP - Functional and Logic Programming**
-   *project*: Project 1, ECDSA (Projekt 1, ECDSA)

## Usage

### Prerequisites

- Haskell compiler (`ghc 9.2.5` was used during development)
- Random library - install with `cabal install --lib random`

### How to run

1. Compile the project
    ```bash
    make
    ```

2. Run the binary
    ```bash
    ./flp22-fun
    ```

Program arguments:

- `-i`: Displays elliptic curve information
- `-k`: Generates new key pairs
- `-s`: Signs messages using provided keys
- `-v`: Verifies signatures

Input for the program is always read from stdin

### Running included tests

> [!NOTE]
> Tests with wiki suffix are based on ECDSA examples from [wikipedia](https://cs.wikipedia.org/wiki/Protokol_digit%C3%A1ln%C3%ADho_podpisu_s_vyu%C5%BEit%C3%ADm_eliptick%C3%BDch_k%C5%99ivek)

#### \_wiki tests

1. generate keypair

```bash
./flp22-fun -k test/test-k-wiki.in
```
example output is in 'test/test-k-wiki.out'

2. sign
```bash
./flp22-fun -s test/test-s-wiki.in
```
example output is in 'test/test-s-wiki.out'

3. verify
```bash
./flp22-fun -v test/test-v-wiki.in
```
expected output: True

-----------------------------

1. generate keypair
```bash
./flp22-fun -k test/test-k.in
```
example output is in 'test/test-k.out'

2. sign
```bash
./flp22-fun -s test/test-s.in
```
example output is in 'test/test-s.out'

3. verify
```bash
./flp22-fun -v test/test-v.in
```
expected output: True

> [!NOTE]
> If you want to see the output of `-i` argument, you can swap it with `-k` argument in the examples above.