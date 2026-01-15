from UINT8BitMask import *
from server import *
from States import *
from music import MusicPlayer

MCAST_GRP = '239.255.0.1'
MCAST_PORT = 5005
server =  MulticastServer(MCAST_GRP, MCAST_PORT)

def start_listener():
    bits = UINT8BitMask()



def stop_listener():
    bits = UINT8BitMask()
    for i in range(ESP_COUNT):
        packet = PacketData(i, bits)
        server.send_packet(packet)


def timed_listener(ms: int):
    if ms not in LIGHTING_STATES:
        return
    data_list = LIGHTING_STATES[ms]
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
        print("\nSent:",packet)
        server.send_packet(packet)


music_player: MusicPlayer = MusicPlayer(start_listener, timed_listener, stop_listener)
music_player.playMusic("song.mp3")