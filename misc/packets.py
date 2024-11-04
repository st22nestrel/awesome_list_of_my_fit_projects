#!/usr/bin/python3.8
import scapy.all as sp

sctp_packet = sp.IP(dst="127.0.0.1",src="127.0.0.1",flags=16384)/sp.SCTP(dport=4064,sport=6066,tag=8963)#/"Simple message"

sp.send(sctp_packet)

#mptcp_packet = sp.IP(dst="127.0.0.1",src="127.0.0.1",flags=16384)/sp.TCP()/sp.TCPOptionsField(name="MPTCP", remain=30)
mptcp_packet = sp.IP(dst="127.0.0.1",src="127.0.0.1",flags=16384)/sp.TCP(options=[(30, "MPTCP")])

sp.send(mptcp_packet)