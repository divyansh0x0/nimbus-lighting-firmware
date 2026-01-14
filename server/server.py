import socket
import struct
from dataclasses import dataclass

from music import MusicPlayer
from UINT8BitMask import UINT8BitMask

import json

from States import LIGHTING_STATES, ESP_COUNT


@dataclass
class PacketData:
    def __init__(self, ecs_id: int,state: UINT8BitMask):
        self.ecs_id = ecs_id
        self.state = state

    def getStruct(self):
        return struct.pack("!BB",self.ecs_id,self.state.bits)
    def __str__(self):
        return "[Packet:"+ str(self.ecs_id) + "," + str(self.state)+"]"

json_file = open("ips.json", "r")
json_text = json_file.read()
json_data = json.loads(json_text)
json_file.close()

IP_ADDRESSES = json_data["IPs"]
PORT = 8080
CLIENTS = set([(ip, PORT) for ip in IP_ADDRESSES])
socket_conn = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)


def send_packet(packet: PacketData):
    for client in CLIENTS:
        socket_conn.sendto(packet.getStruct(), client)


def start_listener():
    bits = UINT8BitMask()
    for i in range(ESP_COUNT):
        packet = PacketData(i, bits)
        send_packet(packet)


def stop_listener():
    bits = UINT8BitMask()
    for i in range(ESP_COUNT):
        packet = PacketData(i, bits)
        send_packet(packet)


def timed_listener(secs: int):
    print(secs)
    if secs not in LIGHTING_STATES:
        return
    data_list = LIGHTING_STATES[secs]
    if not data_list:
        return

    for data in data_list:
        esp_id = data[0]
        esp_states = data[1]
        bits = UINT8BitMask()
        for i in range(len(esp_states)):
            bit_state = True if esp_states[i].lower() == "t" else False
            bits.set_bit(i, bit_state)
        packet = PacketData(esp_id, bits)
        print(packet)
        send_packet(packet)


music_player: MusicPlayer = MusicPlayer(start_listener, timed_listener, stop_listener)
music_player.playMusic("song.mp3")
