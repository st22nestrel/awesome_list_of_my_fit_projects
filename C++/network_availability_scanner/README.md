# Network network availability scanner (L2/L3 scanner)

## About

This project implements a network scanner that performs Layer 2 (ARP) and Layer 3 (ICMPv4) scanning to discover active hosts on a network. It provides functionality to:

- Scan network segments using ARP requests
- Perform ICMP echo requests (ping) on discovered hosts
- Display MAC addresses of responding hosts
- Support for IPv4 address ranges
- Configurable timeout settings

### School subject and project
-   *subject*: **IPK - Computer Communications and Networks**
-   *project*: Project 2, Variant Delta network availability scanner (Projekt 2, Varianta DELTA Scanner sitove dostupnosti)

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

2. Run the binary (requires root privileges)
    ```bash
    sudo ./ipk-l2l3-scan
    ```

Program arguments:

- `-i <interface>`: Network interface to use for scanning
- `-w <wait_time>`: Time in milliseconds to wait for responses (default: 1000)
- `-n <ip_range>`: IP range to scan in CIDR notation (e.g. 192.168.1.0/24)

### Examples

Scan local network on eth0 interface:
```bash
sudo ./ipk-l2l3-scan -i eth0 -n 192.168.1.0/24
```

Scan with custom timeout:
```bash
sudo ./ipk-l2l3-scan -i eth0 -n 192.168.1.0/24 -w 2000
```

### Output

For each scanned IP address, the program outputs:

- IP address
- ARP scan result (OK/FAIL) with MAC address if successful
- ICMP scan result (OK/FAIL)

Example output:
```plaintext
192.168.1.1    arp OK (00-11-22-33-44-55), icmpv4 OK
192.168.1.2    arp FAIL, icmpv4 FAIL
```

## Notes

- IPv6 scanning is not currently implemented
- The scanner requires root privileges to create raw sockets
- ARP scanning is performed first, followed by ICMP for responsive hosts

## Misc

Used resources:
- https://nmap.org/book/host-discovery-techniques.html#host-discovery-ex-msackping
- https://whatismyipaddress.com/cidr
