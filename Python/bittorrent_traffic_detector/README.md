# Bittorrent traffic detector

## About

This project is a BitTorrent DHT (Distributed Hash Table) network monitor that analyzes PCAP (packet capture) files to extract information about BitTorrent DHT network activity. It provides flags:
- `-init` - identifies known BitTorrent bootstrap nodes through DNS queries, tracks initial DHT network entry points and displays discovered bootstrap nodes with their IP addresses and ports 
- `-peers` - tracks DHT peer nodes and their interactions, counts message exchanges between peers, shows peer details including IP, port, node ID, and communication frequency and lastly supports both IPv4 and IPv6 traffic

### School subject and project

-   *subject*: **PDS - Data Communications, Computer Networks and Protocols**
-   *project*: Project: BitTorrent traffic monitoring and detection (Projekt: Monitorování a detekce provozu BitTorrent)

## Usage

### Prerequisites

- `python3` (implemented with `python3.8`)
- python libraries listed in `requirements.txt` (you can use this [manual](../../0-manuals/venv_manual.md) to install them in virtual environment)

### How to run

> [!CAUTION]
> This project is no longer runnable as library `simple_dht_node` was removed from pypi :(

See help message for a list of available command arguments
```bash
./bt-monitor.py --help
# or
python3.8 bt-monitor.py --help
```
