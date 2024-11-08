# KRY project 2 - Hybrid encryption
# Timotej Ponek, xponek00
import socket
from secrets import token_bytes, randbelow
from Crypto.Cipher import AES
from Crypto.PublicKey import RSA
from Crypto.Hash import MD5
from utils import *
from time import sleep

##############################################
## Server
def server(port):
    """Runs the server."""

    server_socket = socket.socket()

    server_socket.bind(("localhost", port))
    server_socket.listen()

    # Accept and handle client connection
    client_socket, client_address = server_socket.accept()
    print('s: "Client has joined"')

    # Load keys
    reciever_public_key = RSA.import_key(open("cert/publicReciever.pem").read())
    reciever_private_key = RSA.import_key(open("cert/privateReciever.pem").read())
    sender_public_key = RSA.import_key(open("cert/publicSender.pem").read())

    print(f"s: RSA_public_key_reciever={hex(reciever_public_key.e)}")
    print(f"s: RSA_private_key_reciever={hex(reciever_private_key.d)}")
    print(f"s: RSA_public_key_sender={hex(sender_public_key.e)}")

    while True:
        ## Get length of data to recieve
        data_len = client_socket.recv(1024)
        nonce = data_len[-16:]
        data_len = int(data_len[:-16].decode())

        rnd_nmb_pad = client_socket.recv(1024)
        rnd_nmb_pad = int(rnd_nmb_pad.decode())

        data_buf = bytearray()

        ## Recieve all data
        while data_len:
            data = client_socket.recv(1024)
            data_buf += data
            data_len -= len(data)

        ## Separate data
        encr_msg = data_buf[:-256]
        encr_aes_key = data_buf[-256:]

        ## AES key decryption
        aes_key = decryptRSA(encr_aes_key, reciever_private_key, rnd_nmb_pad)

        ## Message decryption
        msg_decrypter = AES.new(aes_key, AES.MODE_EAX, nonce = nonce)
        message = msg_decrypter.decrypt(encr_msg)
        plaintext = message[:-512]
        client_hash = message[-512:-256]

        md5_hash = MD5.new(plaintext).digest()

        print(f"s: ciphertext={data_buf.hex()}")
        print(f"s: RSA_AES_key={encr_aes_key.hex()}")
        print(f"s: AES_cipher={encr_msg.hex()}")
        print(f"s: AES_key={aes_key.hex()}")

        print(f"s: text_hash={message.hex()}")
        print(f"s: plaintext={plaintext.decode()}")
        print(f"s: MD5={md5_hash.hex()}")

        if verifyRSA(client_hash, sender_public_key, rnd_nmb_pad, md5_hash):
            print("s: The integrity of the message has not been compromised.")
            client_socket.sendall(b'1')
        else:
            print("s: The integrity of the report has been compromised. Informing client")
            client_socket.sendall(b'0')


##############################################


## Client
def client(port):
    """Runs the client."""

    client_socket = socket.socket()
    client_socket.connect(("localhost", port))
    print("Successfully connected to server")

    ## Load keys
    sender_public_key = RSA.import_key(open("cert/publicSender.pem").read())
    sender_private_key = RSA.import_key(open("cert/privateSender.pem").read())
    reciever_public_key = RSA.import_key(open("cert/publicReciever.pem").read())

    ## Print keys
    print(f"c: RSA_public_key_sender={hex(sender_public_key.e)}")
    print(f"c: RSA_private_key_sender={hex(sender_private_key.d)}")
    print(f"c: RSA_public_key_receiver={hex(reciever_public_key.e)}")

    ## Generate ciphers
    rnd_nmb_pad = randbelow(240)
    
    def clientRoutine(data_size, rnd_nmb_pad, encr_payload):
        while True:
            print(f"c: The message was sent again")
            client_socket.sendall(data_size)
            sleep(0.05)
            client_socket.sendall(rnd_nmb_pad)
            sleep(0.05)
            client_socket.sendall(encr_payload)
            msg_ok = client_socket.recv(1024)
            if msg_ok == b'1':
                print(f"c: The message was succesfully delivered")
                break

    while True:
        ## Enter message
        message = input("c: Enter input: ").encode()

        ## Generate random AES key
        aes_key = token_bytes(16)
        msg_encrypter = AES.new(aes_key, AES.MODE_EAX)

        ## Message hash signing
        md5_hash = MD5.new(message).digest()
        padded_hash = paddingForAsymetric(md5_hash, rnd_nmb_pad, sender_private_key.n) 
        signature = signRSA(md5_hash, sender_private_key)

        ## Message encryption
        encr_msg = msg_encrypter.encrypt(message+signature)

        ## Encrypt AES key with RSA
        padded_key = paddingForAsymetric(aes_key, rnd_nmb_pad, reciever_public_key.n)
        encr_aes_key = encryptRSA(padded_key, reciever_public_key)

        ## Send size of data, random padding and then all data to server
        data_size = len(encr_msg+signature+encr_aes_key)
        client_socket.sendall(str(data_size).encode() + msg_encrypter.nonce)
        sleep(0.05)
        client_socket.sendall(str(rnd_nmb_pad).encode())
        sleep(0.05)
        client_socket.sendall(encr_msg+signature+encr_aes_key)

        print(f"c: AES_key={aes_key.hex()}")
        print(f"c: AES_key_padding={padded_key.hex()}")
        print(f"c: MD5={md5_hash.hex()}")
        print(f"c: MD5_padding={padded_hash.hex()}")
        print(f"c: RSA_MD5_hash={signature.hex()}")
        print(f"c: AES_cipher={encr_msg.hex()}")
        print(f"c: RSA_AES_key={encr_aes_key.hex()}")
        print(f"c: ciphertext={(encr_msg+signature+encr_aes_key).hex()}")

        msg_ok = client_socket.recv(1024)
        if msg_ok == b'1':
            print(f"c: The message was succesfully delivered")
        else:
            clientRoutine(str(data_size).encode() + msg_encrypter.nonce, str(rnd_nmb_pad).encode(), encr_msg+signature+encr_aes_key)



if __name__ == "__main__":
    import argparse

    parser = argparse.ArgumentParser(
                prog='kry',
                description='Client-server hybrid encryption')

    parser.add_argument('TYPE', action = 'store', choices=['c','s'],
                        help='Use `c` to run as client, `s` to run as server.')
    parser.add_argument('PORT', metavar=('{0-65555}'), action = 'store',
                        type=int, choices=range(0, 65555),
                        help='Port on which client/server will be listening.')
    args = None

    args, _ = parser.parse_known_args()

    if (args.TYPE == 'c'):
        client(args.PORT)
    else:
        server(args.PORT)
