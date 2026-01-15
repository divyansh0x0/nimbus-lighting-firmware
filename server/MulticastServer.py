import socket
import struct
import time

MCAST_GRP = '239.255.0.1'
MCAST_PORT = 5005
MESSAGE = b'Hello ESP multicast'

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)

# Limit to local network
sock.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_TTL, 1)

print("Sending multicast packets...")

while True:
    sock.sendto(MESSAGE, (MCAST_GRP, MCAST_PORT))
    time.sleep(1)
