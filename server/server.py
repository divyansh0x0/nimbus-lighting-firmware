import socket
import struct
from UINT8BitMask import UINT8BitMask
import json

class PacketData:
    def __init__(self, ecs_id: int,state: UINT8BitMask):
        self.ecs_id = ecs_id
        self.state = state

    def getStruct(self):
        return struct.pack("!BB",self.ecs_id,self.state.bits)
    def __str__(self):
        return "[Packet:"+ str(self.ecs_id) + "," + str(self.state)+"]"





class MulticastServer:
    def __init__(self, ip:str,port:int):
        self.addr = (ip,port)
        self.socket_conn = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
        self.socket_conn.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_TTL, 1)

    def send_packet(self, packet:PacketData):
        self.socket_conn.sendto(packet.getStruct(), self.addr)
