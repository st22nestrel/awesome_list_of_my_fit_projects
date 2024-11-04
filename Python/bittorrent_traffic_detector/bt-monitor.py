#!/usr/bin/env python3
# project for PDS course
# author: Timotej Ponek, xponek00, VUT FIT

from scapy.all import *
import dht_node.utils as dht
from dht_node.data_structures import Node
from ipaddress import ip_address
import bencodepy

###############################################
def parse_compact_node_info_ipv6(data: bytes) -> List[Node]:
    """Convert compact node info string (BEP 5) to Node objects"""
    result_nodes = []
    for node_index in range(len(data) // 38):
        current_data = data[38 * node_index : 38 * (node_index + 1)]  # noqa
        _id, _ip, port = struct.unpack("!20s16sH", current_data)
        result_nodes.append(
            Node(id=_id.hex(), ip=str(ip_address(_ip)), port=port)
        )
    return result_nodes
###############################################

args = None

known_bootstrap_nodes = set([b"router.utorrent.com",
b"router.bittorrent.com", b"dht.transmissionbt.com",
b"router.bitcomet.com", b"dht.aelitis.com",
b"dht.libtorrent.org", b"dht.vuze.com"])

initNodes = dict()
initNodesAllSet = False

bootstrapNodes = dict()
peerNodes = dict()

def checkInSet(value: bytes, check_set: set) -> bool:
    for key in check_set:
        if value.startswith(key):
            return True

def checkDictAllSet(check_dict: dict) -> bool:
    for key, val in check_dict.items():
        if val is None:
            return False

def printInit():
    for key, val in bootstrapNodes.items():
        print(f"ip: {key}; port: {val}")

def printPeers():
    sorted_peerNodes = sorted(peerNodes.items(), key=lambda x: x[1][1], reverse=True)
    for key, (node, com_nmb) in sorted_peerNodes:
        print(f"ip: {node.ip}; port: {node.port}; id: {key}; ", end="")
        print("no messages exchanged with peer" if com_nmb == 0 else f'nmb of messages exchanded: {com_nmb}')

def process():
    for pkt in PcapReader(args.pcap):

        # search for bootstrap servers IPs
        if args.init and DNS in pkt:
            dns_an = pkt[DNS].an
            if (dns_an and
            checkInSet(dns_an.rrname, known_bootstrap_nodes)):
                # A and AAAA
                if (dns_an.type == 1 or dns_an.type == 28):
                    # this would insert ip + dns_name
                    initNodes.update({dns_an.rdata: None})
                # we got an alias for known bootstrap node
                elif (dns_an.type == 5):
                    known_bootstrap_nodes.add(dns_an.rdata)

        # only possible bt-dht packets
        if (IP in pkt and UDP in pkt and Raw in pkt
            and not DNS in pkt):
            #"INIT IP"
            if args.init and not initNodesAllSet:
                if pkt[IP].src in initNodes.keys():
                    initNodes.update({pkt[IP].src: pkt[UDP].sport})
                    checkDictAllSet(initNodes)
                    decoded_payload = bencodepy.decode(pkt[Raw].load)
                    peer_nodes = dht.parse_compact_node_info(decoded_payload.get(b'r').get(b'nodes'))
                    # add those nodes as bootstrap nodes
                    [bootstrapNodes.update({x.ip: x.port}) for x in peer_nodes if bootstrapNodes.get(x.id, True)]
                    # also add them to peers
                    [peerNodes.update({x.id: (x, 0)}) for x in peer_nodes if peerNodes.get(x.id, True)]
            
            #"Only peers IP"
            elif args.peers:
                # update peer list
                decoded_payload = None
                try:
                    decoded_payload = bencodepy.decode(pkt[Raw].load)
                except:
                    continue
                
                #break if wrong message
                if decoded_payload.get(b'r') is None or decoded_payload.get(b'r').get(b'nodes') is None:
                    continue
                
                peer_nodes = dht.parse_compact_node_info(decoded_payload.get(b'r').get(b'nodes'))
                [peerNodes.update({x.id: (x, 0)}) for x in peer_nodes if peerNodes.get(x.id, True)]

                # check who you are comunicating with
                id_src_peer = dht.get_node_id(decoded_payload)

                peer = peerNodes.get(id_src_peer)
                if peer is not None:
                    node, com_nmb = peer
                    peerNodes.update({id_src_peer: (node, com_nmb + 1)})
                else:
                    peerNodes.update({id_src_peer: (Node(id_src_peer, pkt[IP].src, pkt[UDP].sport), 1)})

        # do the same for IPv6 packets
        if (IPv6 in pkt and UDP in pkt and Raw in pkt
            and not DNS in pkt):
            #"INIT IPv6"
            if args.init and not initNodesAllSet:
                if pkt[IPv6].src in initNodes.keys():
                    initNodes.update({pkt[IPv6].src: pkt[UDP].sport})
                    checkDictAllSet(initNodes)
                    decoded_payload = bencodepy.decode(pkt[Raw].load)
                    peer_nodes = parse_compact_node_info_ipv6(decoded_payload.get(b'r').get(b'nodes6'))
                    # add those nodes as bootstrap nodes
                    [bootstrapNodes.update({x.ip: x.port}) for x in peer_nodes if bootstrapNodes.get(x.id, True)]
                    # also add them to peers
                    [peerNodes.update({x.id: (x, 0)}) for x in peer_nodes if peerNodes.get(x.id, True)]
            
            #"Only peers IPv6"
            elif args.peers:
                # update peer list
                decoded_payload = None
                try:
                    decoded_payload = bencodepy.decode(pkt[Raw].load)
                except:
                    continue

                #break if wrong message
                if decoded_payload.get(b'r') is None or decoded_payload.get(b'r').get(b'nodes') is None:
                    continue

                peer_nodes = parse_compact_node_info_ipv6(decoded_payload.get(b'r').get(b'nodes6'))
                [peerNodes.update({x.id: (x, 0)}) for x in peer_nodes if peerNodes.get(x.id, True)]

                # check who you are comunicating with
                id_src_peer = dht.get_node_id(decoded_payload)

                peer = peerNodes.get(id_src_peer)
                if peer is not None:
                    node, com_nmb = peer
                    peerNodes.update({id_src_peer: (node, com_nmb + 1)})
                else:
                    peerNodes.update({id_src_peer: (Node(id_src_peer, pkt[IPv6].src, pkt[UDP].sport), 1)})


if __name__ == "__main__":
    import argparse

    parser = argparse.ArgumentParser(
                prog='bt-dh',
                description='Client-server hybrid encryption')

    parser.add_argument('-pcap', metavar=('<file.pcap>'), action = 'store',
                        help='input PCAP file or <file.csv> input CSV file',
                        required=True)
    parser.add_argument('-init',action = 'store_true',
                        help='returns a list of detected bootstrap nodes (IP, port)')
    parser.add_argument('-peers',action = 'store_true',
                        help=' returns a list of detected neighbors (IP, port, node ID, # of conn)')
    parser.add_argument('-download', action = 'store_true',
                        help='returns file info_hash, size, chunks, contributes (IP+port)')
    parser.add_argument('-rtable', action = 'store_true',
                        help='returns the routing table of the client (node IDs, IP, ports)')

    args, _ = parser.parse_known_args()

    process()

    if args.init:
        print("-------------INIT--------------")
        printInit()
    if args.peers:
        print("-------------PEERS-------------")
        printPeers()
