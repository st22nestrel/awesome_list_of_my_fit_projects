# Spanning tree searcher

## About

This project implements a spanning tree search algorithm that finds all possible spanning trees in a given graph.

### School subject and project

- *subject*: **FLP - Functional and Logic Programming**
- *project*: Project 2, Spanning Tree (Projekt 2, Kostra grafu)

## Usage

### Prerequisites

- SWI-Prolog interpreter (used for compilation)


### How to run

1. Compile the project
    ```bash
    make
    ```

2. Run the binary
    ```bash
    ./flp22-log
    ```

Program reads the input graph from stdin and outputs all possible spanning trees for the given graph.

### Input format

The input should describe a graph in the following format:
- Each input line represents one edge
- Input is read until EOF

Example input:
```
A B
A C
```

### Output

The program will output all possible spanning trees found in the input graph.

### Running tests

Test files can be found in the `test/` directory.

1. 
```bash
./flp22-log < test/example.in
```
example output is in 'test/example.out'

2. 
```bash
./flp22-log < test/example2.in
```
example output is in 'test/example2.out'

3. 
```bash
./flp22-log < test/example3.in
```
example output is in 'test/example3.out'

4. 
```bash
./flp22-log < test/example4.in
```
example output is in 'test/example4.out'

5. 
```bash
./flp22-log < test/example_oriented.in
```
example output is in 'test/example_oriented.out'

## Misc

- [Algorithms for generating all possible spanning trees of a simple undirected connected graph](https://link.springer.com/article/10.1007/s40747-018-0079-7)
- [Prolog commenting style](https://people.cs.vt.edu/~ryder/5314/Projects/Prolog/SEdwardsPrologComments.pdf)