# FSP File Download Client

## About

This project implements a file download client that uses a custom FSP (File Service Protocol) to:

- Communicate with a name server to resolve file server locations
- Download individual files from FSP servers
- Download all files from an FSP server using wildcard pattern
- Handle directory creation for nested file paths

### School subject and project

-   *subject*: **IPK - Computer Communications and Networks**
-   *project*: Project 1 (Projekt 1)

## Usage

### Prerequisites

- Python 3.8 or higher

### How to run

The program accepts two mandatory arguments:

```bash
python3.8 fileget.py -n NAMESERVER -f SURL
```

Program arguments:

- `-n NAMESERVER`: IP address and port of the name server in format `ip:port`
- `-f SURL`: SURL (Service URL) of the file to download. Protocol must be `fsp://`

### Examples

> [!IMPORTANT]  
> Bellow are just examples of how arguments passed to the program should look like, not a working examples. 
> You have to find existing fsp server (`-f` argument) from which you would like to download a file/files by yourself.

1. Download a single file:
```bash
python3.8 fileget.py -n 127.0.0.1:3333 -f fsp://files.example.com/test.txt
```

2. Download all files from server:
```bash
python3.8 fileget.py -n 127.0.0.1:3333 -f fsp://files.example.com/*
```


## Protocol Details

The client implements a two-step process:

1. Name Server Communication:

    - Sends WHEREIS hostname to get file server address
    - Receives server address in format OK ip:port

2. File Server Communication:

    - Connects using TCP
    - Sends GET request in format:
        ```plaintext
        GET path FSP/1.0
        Hostname: hostname
        Agent: xponek00
        ```
    - Receives and stores requested files

## Error Handling
The program handles these error cases:

- Missing or invalid arguments
- Name server communication failures
- File server connection timeouts
- Invalid protocol specifications
- File system operations
- All errors are reported to stderr with appropriate error messages.
