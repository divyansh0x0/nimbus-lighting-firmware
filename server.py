import socket
import struct
import time
import re

ESP_IP = input("IP: ") or "10.196.39.31"
ESP_PORT = int(input("Port: ") or "8080")

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

pattern = """
1s 01101
3s 01001
""".strip()


def parse_pattern(pattern_text):
    """
    Returns list of (time_in_seconds, bit_string)
    """
    events = []
    for line in pattern_text.splitlines():
        line = line.strip()
        if not line:
            continue

        match = re.match(r"(\d+)s\s+([01]{1,5})", line)
        if not match:
            raise ValueError(f"Invalid pattern line: {line}")

        t = int(match.group(1))
        bits = match.group(2)
        events.append((t, bits))

    return events


def apply_bits(old_state, new_bits):
    """
    old_state: list of 5 ints
    new_bits: string of 1 to 5 bits
    Missing bits keep old value
    """
    new_state = old_state[:]
    for i, bit in enumerate(new_bits):
        new_state[i] = int(bit)
    return new_state


def state_to_byte(state_bits):
    """
    [s1,s2,s3,s4,s5] -> single byte
    """
    value = 0
    for bit in state_bits:
        value = (value << 1) | bit
    return value


def send_state(state_bits):
    byte = state_to_byte(state_bits)
    packet = struct.pack("!B", byte)
    sock.sendto(packet, (ESP_IP, ESP_PORT))
    print(f"Sent -> {''.join(map(str, state_bits))} ({byte})")


if __name__ == "__main__":
    print("UDP 5-bit Pattern Sender")
    print("------------------------------")

    events = parse_pattern(pattern)

    state = [0, 0, 0, 0, 0]   # initial 00000
    start_time = time.time()
    last_event_time = 0

    for event_time, bits in events:
        wait_time = event_time - last_event_time
        if wait_time > 0:
            time.sleep(wait_time)

        state = apply_bits(state, bits)
        send_state(state)

        last_event_time = event_time

    print("Pattern complete.")
