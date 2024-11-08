# IPPcode21 XML Interpreter

## About

This project implements an interpreter for IPP2021 language stored in XML format. It provides functionality to:

- Parse XML formatted IPP2021 code
- Interpret the parsed code
- Execute instructions according to IPP2021 specification
- Handle variables, labels, and program flow control

Project Structure:

- `interpret.py` - Main interpreter implementation
- `parsePy.py` - XML parsing functionality
- `dataStructs.py` - Data structures for interpreter
- `test.php` - Testing framework
- `testArgs.php` - Command line argument handling

### School subject and project

-   *subject*: **IPP - Principles of Programming Languages**
-   *project*: Project 2, XML Interpreter (Projekt 2, XML Interpret)

## Usage

### Prerequisites

- Python 3.8 or higher
- PHP 7.4 or higher

### How to run

1. Run the interpreter:
    ```bash
    python3.8 interpret.py --source=input.xml [--input=input_file]
    ```

2. Run tests:
    ```bash
    php test.php [options]
    ```

Program arguments for interpreter:

- `--source=file` - Input XML file with IPP2021 code
- `--input=file` - File with inputs for the interpreted program

Program arguments for tests:

- `--directory=path` - Directory with test files
- `--recursive` - Recursively search for tests
- `--parse-script=file` - Path to parser script
- `--int-script=file` - Path to interpreter script
- `--parse-only` - Run only parser tests
- `--int-only` - Run only interpreter tests
- `--jexamxml=path` - Path to JExamXML JAR file
- `--jexamcfg=file` - Path to JExamXML configuration

#### Test File Structure

Tests expect following files:

- `.src` - Source code
- `.in` - Input file
- `.out` - Expected output
- `.rc` - Expected return code

### Testing

Run the complete test suite:
```bash
php test.php --directory=tests --recursive
```

Run interpreter-only tests:
```bash
php test.php --directory=tests --int-only
```

Run parser-only tests:
```bash
php test.php --directory=tests --parse-only
```

The test framework provides HTML output with detailed test results and statistics.