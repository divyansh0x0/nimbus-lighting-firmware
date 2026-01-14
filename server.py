import socket
import struct
from enum import Enum
import time
import re
from typing import Dict

server_ip = "0.0.0.0"
server_port = "8080"
BUFFER_SIZE = 1024

NO_OF_ESP_IN_NETWORK = 3
ServerSocket = None
isMusicStarted = False
start_time = None
MAX_NO_OF_LED_STRIP_IN_ESP = 5

class LEDSTATE(Enum):
    POWERON = 1
    POWEROFF = 0

class Esp(object):
    
    def __init__(self, node_id:int, no_of_led_strip_in_it:int, ip_address):
        self.node_id = node_id
        self.no_of_led_strip_in_it = no_of_led_strip_in_it
        self.ip_address = ip_address
        self.last_state = None

    def send_command(self, ledstate:LEDSTATE, led_strip_nos:list):
        if len(self.led_strip_nos) > self.no_of_led_strip_in_it:
            raise Exception("LED strip out of range")
        fmt = "<B"
        state_to_deploy = self.create_leds_state(ledstate, led_strip_nos)
        packet = struct.pack(fmt, state_to_deploy)
        ServerSocket.sendto(packet, self.ip_address)
        self.last_state = state_to_deploy
    
    def create_led_state(self, led_list: list[int], state: LEDSTATE) -> int:
        led_state = self.last_state
        for led in led_list:
            if state:
                led_state |= (1 << (led-1))     # turn ON
            else:
                led_state &= ~(1 << (led-1))    # turn OFF
        return led_state

Espobj:Dict[int, Esp] = {}

try:
        # AF_INET specifies the IPv4 address family
        # SOCK_DGRAM specifies the User Datagram Protocol (UDP)
        ServerSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)
        ServerSocket.bind((server_ip, server_port))
        print(f"UDP server up and listening on {server_ip}:{server_port}")
except socket.error as e:
        print(f"Error binding socket: {e}")
        raise

my_pattern = [
[1, LEDSTATE.POWERON, {1: [1,2,3]}]
[3, LEDSTATE.POWERON, {1: [4,5,6]}],
[4, LEDSTATE.POWEROFF, {1:[4,2,6]}],
[7, LEDSTATE.POWEROFF, "ALL"]
]

def get_time():
    return time.time() - start_time

def reset_time():
    start_time = time.time()

if __name__ == "__main__":
    print("UDP Control Server")
    print("------------------------------")
    print("Commands:")
    print("<stripId> <state>   (example: 12345 3 1)")
    print("  q                     quit")
    print("------------------------------")
    while True:
        client_esp = ServerSocket.recvfrom(BUFFER_SIZE)
        node_id = struct.unpack("<B", client_esp[0])
        Espobj[node_id] = Esp(node_id, MAX_NO_OF_LED_STRIP_IN_ESP, client_esp[1])
        
        if NO_OF_ESP_IN_NETWORK == len(Espobj):
            print("All esp are connected now")
            break
    
    curr_command_index = 0
    while True:
        if not isMusicStarted:
            continue

        if curr_command_index == 0:
            start_time = time.time()

        for command in my_pattern.split("\n"):
            time_to_fire_comm, led_state,led_to_change = command[0], command[1], command[2]

            if time_to_fire_comm <= get_time():
                for i in led_to_change:
                    Espobj[i].send_command(led_to_change, led_state,led_to_change[i])

