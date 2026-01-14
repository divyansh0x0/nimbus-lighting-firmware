import socket
import struct
import time
import re

ESP_IP = input("IP: ") or "10.196.39.31"
ESP_PORT = int(input("Port: ") or "8080")

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

class Packet:
    def __init__(self):
        self.id:int = 0
        self.line_status

def send_state(state_bits):
    byte = state_to_byte(state_bits)
    packet = struct.pack("!B", byte)
    sock.sendto(packet, (ESP_IP, ESP_PORT))
    print(f"Sent -> {''.join(map(str, state_bits))} ({byte})")


