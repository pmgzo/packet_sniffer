# packet sniffer

this is a minimal packet sniffer, which helped me to understand and use raw socket

## definition

A packet sniffer is a program which catch transiting network packet in a network.
A good exemple of packet sniffer is Wireshark.

To implement this script I simply used that webpage: https://www.opensourceforu.com/2015/03/a-guide-to-using-raw-sockets/

## raw socket

It exists different types of socket in C, raw socket enable us to catch packet but also to create packet, if I'm not wrong you can decide on both receiver and sender when creating the packet.
It is quite appealing actually.

## UDP, TCP

This implementation helped me to understand how works TCP and UDP protocol as we should parse or build tcp and udp headers & packet when using raw socket
