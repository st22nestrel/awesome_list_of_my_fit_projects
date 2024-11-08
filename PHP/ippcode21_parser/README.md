# IPPcode21 Parser

## About

This project is a parser implementation that transforms IPPcode21 programs into xml representation. It provides functionality to:

- Parse IPPcode21 source code
- Generate XML output
- Collect various code statistics
- Verify code structure and syntax
- Handle labels and jumps

### School subject and project

-   *subject*: **IPP - Principles of Programming Languages**
-   *project*: Project 1, Parser (Projekt 1, Parser)

## Usage

### Prerequisites

- PHP 7.4 or higher (implemented with PHP 7.4)
- PHP CLI

### How to run

Run the parser
```bash
php parse.php [options] < input_file
```

Program arguments:

`--help`: Display help message
`--stats=file`: Output statistics to specified file
`--loc`: Include lines of code count in statistics
`--comments`: Include comment count in statistics
`--labels`: Include label count in statistics
`--jumps`: Include jump instruction count
`--fwjumps`: Include forward jump count
`--backjumps`: Include backward jump count
`--badjumps`: Include count of jumps to undefined labels

### Testing

The project includes a test framework (testArgs.php) that can verify parser functionality:
```bash
php testArgs.php [options]
```

Test framework options:

- `--help`: Display help message
- `--directory="path"`: Search for tests in given directory
- `--recursive`: Search in subdirectories
- `--parse-script="file"`: Path to parse.php
- `--int-script="file"`: Path to interpret.py
- `--parse-only`: Test only parser functionality
- `--int-only`: Test only interpreter functionality
- `--testlist="file"`: Specify test files explicitly
- `--match="regex"`: Filter tests by regex pattern