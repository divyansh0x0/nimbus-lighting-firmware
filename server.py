import socket
import struct

ESP_IP = "192.168.1.50"   # <-- change to ESP8266 IP
ESP_PORT = 4210

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

def send_command(time_sec: int, cmd_id: int, state: int):
    # Ensure time wraps as uint16
    time_sec = time_sec & 0xFFFF

    # !  -> network (big endian)
    # H  -> uint16
    # B  -> uint8
    # B  -> uint8
    packet = struct.pack("!HBB", time_sec, cmd_id, state)
    sock.sendto(packet, (ESP_IP, ESP_PORT))
    print(f"Sent -> time={time_sec}, id={cmd_id}, state={state}")

if __name__ == "__main__":
    print("UDP Control Server")
    print("Commands:")
    print("  <time> <id> <state>   (example: 12345 3 1)")
    print("  q                     quit")

    while True:
        cmd = input("> ").strip()
        if cmd.lower() == "q":
            break

        try:
            time_sec, cmd_id, state = map(int, cmd.split())
            if not (0 <= time_sec <= 65535):
                raise ValueError("Time must be 0-65535 (uint16)")
            if not (0 <= cmd_id <= 254):
                raise ValueError("ID out of range")
            if state not in (0, 1):
                raise ValueError("State must be 0 or 1")

            send_command(time_sec, cmd_id, state)

        except ValueError as e:
            print("Invalid input:", e)
