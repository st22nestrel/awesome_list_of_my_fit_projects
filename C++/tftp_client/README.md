# TFTP Client

## About

This project implements a TFTP (Trivial File Transfer Protocol) client that provides functionality to:

- Download files from TFTP servers
- Upload files to TFTP servers
- Support for both IPv4 and IPv6 addresses
- Support for ASCII/NetASCII and binary transfer modes
- Support for multicast transfers
- Configurable timeout and segment size

### School subject and project
-   *subject*: **ISA - Network Applications and Network Administration**
-   *project*: TFTP Client (TFTP Klient)

## Usage

### Prerequisites

- Linux operating system
- C++ compiler with C++11 support
- Project must be run with root privileges (required for raw sockets)

### How to run

1. Compile the project
    ```bash
    make
    ```

2. Run the binary
    ```
    sudo ./tftp-client
    ```

The client runs in interactive mode, waiting for commands from stdin.

### Command Format

Commands should be provided in the following format:

Required arguments:

- `-R` or `-W`: Read (download) or Write (upload) operation
- `-d <path>`: File path for download/upload

Optional arguments:

- `-t <timeout>`: Custom timeout for packet requests
- `-s <size>`: Custom transfer size (data packet portion)
- `-m`: Request multicast transfer (only available with -R)
- `-c <mode>`: Transfer mode - 'netascii' or 'binary'
- `-a <address>[,port]`: IPv4/IPv6 address with optional port (default: 69)

### Examples

1. Download a file in binary mode:
    ```plaintext
    -R -d myfile.txt -c binary -a 192.168.1.100
    ```

2. Upload a file with custom timeout and segment size:
    ```plaintext
    -W -d upload.txt -t 5 -s 1024 -a 192.168.1.100,69
    ```

3. Download using multicast:
    ```plaintext
    -R -d download.txt -m -a 192.168.1.100
    ```

### Error Handling
The client provides detailed error messages with timestamps for:

- Connection issues
- Transfer errors
- Invalid arguments
- Protocol errors

All error messages are printed to stderr with timestamps in the format: `[YYYY-DD-MM HH:MM:SS.mmm] Error message`

## Notes

- If port is not specified in the address argument, default port 69 is used
- The client supports both IPv4 and IPv6 addresses
- For ASCII mode transfers, the client handles CR/LF conversions automatically
- Multicast transfers are only supported for download operations (-R)

## Misc
- https://www.linkedin.com/pulse/tftp-client-implementation-c-sumit-jha/