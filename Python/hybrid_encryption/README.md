# Hybrid Encryption System

## About

This project implements a client-server hybrid encryption system that combines symmetric and asymmetric encryption to provide:

- Secure message exchange using AES for symmetric encryption
- RSA for key exchange and digital signatures
- Message integrity verification using MD5 hashing
- Real-time client-server communication

### School subject and project

- *subject*: **KRY - Cryptography**
- *project*: Project 2, Hybrid Encryption (Projekt 2, Hybridní šifrování)

## Usage

### Prerequisites

- Python 3 (python 3.8 used for development)
- libraries listed in `requirements.txt`
    - run `make build` to install them in venv (you can also have a look at [venv manual](../../0-manuals/venv_manual.md))
- openssl command line tool (for RSA keys generation)
- rsa keys: steps to generate them:
    1. First create the cert directory:
        ```bash
        mkdir cert
        ```

    2. Generate the keys using OpenSSL:
        ```bash
        # private key sender (client)
        openssl genrsa -out cert/privateSender.pem 2048
        # public key sender (client)
        openssl rsa -in cert/privateSender.pem -pubout -out cert/publicSender.pem
        # public key reciever (server)
        openssl genrsa -out cert/privateReciever.pem 2048
        # private key reciever (server)
        openssl rsa -in cert/privateReciever.pem -pubout -out cert/publicReciever.pem
        ```

### How to run

The system can operate in two modes - client or server.

1. Start the server:
    ```bash
    python3 kry.py s PORT
    # or you can use make run
    make run s PORT
    ```

2. Start the client:
    ```bash
    python3 kry.py c PORT
    # or you can use make run
    make run c PORT
    ```

Program arguments:

- First argument: Mode selection
    - `s`: Run as server
    - `c`: Run as client
    
- Second argument: Port number (0-65554)

### System Operation

1. Server mode:

    - Listens for incoming connections
    - Decrypts received messages using hybrid encryption
    - Verifies message integrity
    - Sends confirmation to client

2. Client mode:
    - Connects to server
    - Encrypts user input using AES
    - Signs messages using RSA
    - Handles message delivery confirmation
    
### Key Management

The system expects the following key files in the cert/ directory:

- publicReciever.pem
- privateReciever.pem
- publicSender.pem
- privateSender.pem

### Communication Protocol

1. Client generates AES key for message encryption
2. Message is hashed using MD5 and signed with sender's private key
3. AES key is encrypted using receiver's public key
4. Encrypted message, signature, and encrypted AES key are sent to server
5. Server verifies message integrity and sends confirmation

> [!NOTE] 
> The system automatically retries message delivery if integrity verification fails

> [!NOTE] 
> For security purposes, ensure proper key management and keep private keys secure