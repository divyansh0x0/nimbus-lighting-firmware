import socket
import struct

ESP_IP = input("IP:") or "10.196.39.137"   # <-- change to ESP8266 IP
ESP_PORT = int(input("Port: ") or "8080")

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
my_pattern = """
1s ON 1
2s ON 2
3s OFF 1,2
4s ON 1,2
5s OFF 1
6s OFF 2 
""".strip()
def send_command(cmd_id: int, state: int):
    # Ensure time wraps as uint16

    # !  -> network (big endian)
    # B  -> uint8
    # B  -> uint8
    packet = struct.pack("!BB", cmd_id, state)
    sock.sendto(packet, (ESP_IP, ESP_PORT))
    print(f"Sent -> id={cmd_id}, state={state}")

if __name__ == "__main__":
    print("UDP Control Server")
    print("------------------------------")
    print("Commands:")
    print("<stripId> <state>   (example: 12345 3 1)")
    print("  q                     quit")
    print("------------------------------")
    while True:
        cmd = input("> ").strip()
        if cmd.lower() == "q":
            break

        try:
            ledStripId, state = map(int, cmd.split())
            if not (0 <= ledStripId <= 254):
                raise ValueError("ID out of range")
            if state not in (0, 1):
                raise ValueError("State must be 0 or 1")

            send_command(ledStripId, state)

        except ValueError as e:
            print("Invalid input:", e)
