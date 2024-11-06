# Dirgraph - Directory Analyzer

## About

Dirgraph is a shell script that generates a histogram visualization of file sizes in a directory. It provides functionality to:

- Analyze file size distribution in directories
- Generate visual histograms using ASCII characters
- Filter files using regular expressions
- Normalize histogram output to terminal width

### School subject and project

- *subject*: **IOS - Operating Systems**
- *project*: Project 1 (Projekt 1)

## Usage

### Prerequisites

- POSIX-compliant shell

### How to Run

1. Make the script executable:
    ```bash
    chmod +x dirgraph
    ```

2. Run the script:
    ```bash
    ./dirgraph [OPTIONS] [DIRECTORY]
    ```

### Program Arguments

- `-i ERE`: Ignores files matching the extended regular expression pattern
- `-n`: Normalizes the histogram to the terminal width
- `DIRECTORY`: Optional path to analyze (defaults to current directory if not specified)

### Output

The script displays:

- Root directory path
- Number of directories
- Total number of files
- File size histogram with the following categories:
    - \<100 B
    - \<1 KiB
    - \<10 KiB
    - \<100 KiB
    - \<1 MiB
    - \<10 MiB
    - \<100 MiB
    - \<1 GiB
    - \>=1 GiB

### Example Usage

1. Analyze current directory:
    ```bash
    ./dirgraph
    ```

2. Analyze specific directory with normalized output:
    ```bash
    ./dirgraph -n /path/to/directory
    ```

3. Analyze directory while ignoring certain files:
    ```
    ./dirgraph -i "\.git" /path/to/directory
    ```

## Notes
- The histogram uses '#' characters to represent file counts
- When using the `-n` option, the histogram scales to fit the terminal width
- Error messages are directed to stderr
- File size categories are displayed in human-readable formats (B, KiB, MiB, GiB)