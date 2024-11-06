#!/usr/bin/env python3.8
# author Timotej Ponek, xponek00
# First project for IPK course, VUT FIT Brno

import socket
import argparse
from urllib.parse import urlparse
import os
from sys import stderr
import re

def recieveAndStore(dir):
    global client2
    justRecieve = False
    try:
        b = client2.recvmsg(1024)
    except:
        print('ERR: failed to recieve from the server - timed out', file=stderr)
        client2.close()
        exit(1)

    allFun = b[0].decode(errors='ignore').split('\r\n', 3)
    msg = bytearray()

    succes = re.search(r'Success', allFun[0])
    if succes is None:
        print('Error {err}'.format(allFun[0]))
        justRecieve = True
    if len(allFun) == 4:
        msg.extend(allFun[3].encode())
    while True:
        try:
            b = client2.recvmsg(1024)
        except:
            print('ERR: failed to recieve from the server - timed out', file=stderr)
            client2.close()
            exit(1)
        msg.extend(b[0])
        if b[0] == b'':
            break

    if(justRecieve):
        return
    # save recieved file
    isDir = os.path.dirname(dir.lstrip('/'))
    if isDir:
        os.makedirs(isDir, exist_ok=True)
    f = open(dir.lstrip('/'), 'wb+')
    f.write(msg)
    f.close()
    return

parser = argparse.ArgumentParser(description='fileget')
parser.add_argument('-n', metavar=('NAMESERVER'), help='IP adresa a číslo portu jmenného serveru')
parser.add_argument('-f', metavar=('SURL'), help='SURL souboru pro stažení. Protokol v URL je vždy fsp')

args, _ = parser.parse_known_args()

if(args.n is None or args.f is None):
    print('ERR: missing arguments', file=stderr)
    exit(1)
surl = urlparse(args.f)
if(surl.scheme != 'fsp'):
    print('ERR: unexpected protocol, expected fsp', file=stderr)
    exit(1)

# first socket creation
socket.setdefaulttimeout(30)
client = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
nameServer = args.n.split(':')

# connect to nameserver and send request for adress
try:
    client.connect((nameServer[0], int(nameServer[1])))
    getMsg = f'WHEREIS {surl.hostname}'
    client.sendall(getMsg.encode())
    where = client.recvmsg(1024)
except:
    client.close()
    print('ERR: Failed to get adress of server, check if you entered correct arguments', file=stderr)
    exit(1)

whereTo = where[0].decode()
client.close()
# check if we got adress
errCode = whereTo[:3]
if(errCode == 'ERR'):
    print(f'ERR: {whereTo[4:]}', file=stderr)
    exit(1)

whereTo = whereTo[3:].split(':')

# second socket creation
client2 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

getAll = False
# we do not suppport single download of files named as '*',
if(surl.path=='/*'):
    getAll = True
    getMsg = f'GET index FSP/1.0\r\nHostname: {surl.hostname}\r\nAgent: xponek00\r\n\r\n'
else:
    getMsg = 'GET {0} FSP/1.0\r\nHostname: {1}\r\nAgent: xponek00\r\n\r\n'.format(surl.path.lstrip('/'), surl.hostname)

client2.connect((whereTo[0], int(whereTo[1])))

# send message to get the file/index
try:
    client2.sendall(getMsg.encode())
except:
    print('ERR: failed to send request to the server', file=stderr)
    client2.close()
    exit(1)

if getAll:
    try:
        b = client2.recvmsg(1024)
    except:
        print('ERR: failed to recieve from the server - timed out', file=stderr)
        client2.close()
        exit(1)

    allFun = b[0].decode().split('\r\n', 3)

    msg = bytearray()
    if len(allFun) == 4:
        msg.extend(allFun[3].encode())

    msg.extend(allFun[3].encode())
    while True:
        try:
            b = client2.recvmsg(1024)
        except:
            print('ERR: failed to recieve from the server - timed out', file=stderr)
            client2.close()
            exit(1)
        msg.extend(b[0])
        if b[0] == b'':
            break

    msg = msg.decode()
    indexFiles = msg.split('\r\n')[:-1]

    for fileTmp in indexFiles:
        getMsg = f'GET {fileTmp} FSP/1.0\r\nHostname: {surl.hostname}\r\nAgent: xponek00\r\n\r\n'
        
        try:
            client2 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            client2.connect((whereTo[0], int(whereTo[1])))
            client2.sendall(getMsg.encode())
        except:
            print('ERR: failed to send request to the server - timed out', file=stderr)
            client2.close()
            exit(1)

        recieveAndStore(fileTmp)
else:
    recieveAndStore(surl.path)

client2.close()