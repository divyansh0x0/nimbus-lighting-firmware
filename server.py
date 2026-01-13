import socket
import struct
import time

ESP_IP = "192.168.1.50"   # <-- change to ESP8266 IP
ESP_PORT = 4210

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

def send_command(cmd_id: int, state: int):
    # uint16 seconds (wraps at 65536)
    time_sec = int(time.time()) & 0xFFFF

    # !  -> network (big endian)
    # H  -> uint16
    # B  -> uint8
    # B  -> uint8
    packet = struct.pack("!HBB", time_sec, cmd_id, state)

    sock.sendto(packet, (ESP_IP, ESP_PORT))
    print(
        f"Sent -> time={time_sec}, id={cmd_id}, state={state}"
    )

if __name__ == "__main__":
    print("UDP Control Server")
    print("Commands:")
    print("  <id> <state>   (example: 3 1)")
    print("  q              quit")

    while True:
        cmd = input("> ").strip()
        if cmd.lower() == "q":
            break

        try:
            cmd_id, state = map(int, cmd.split())
            if not (0 <= cmd_id <= 254):
                raise ValueError("ID out of range")
            if state not in (0, 1):
                raise ValueError("State must be 0 or 1")

            send_command(cmd_id, state)

        except ValueError as e:
            print("Invalid input:", e)
