import socket
import time
import board
import neopixel
import json
import collections
import random
import statistics

# Socket Parameters
HOST = "0.0.0.0"
PORT = 8080

# LED Parameters
NUM_LEDS = 50
LED_PIN = board.D18
ORDER = neopixel.RGB
AMPLITUDE = 0.009
COLOR = (255,200,0)

# Order Parameter Parameters
HISTORY_SECONDS = 2
FPS = 20
MAX_HISTORY = HISTORY_SECONDS * FPS

# Initialize LEDs
leds = neopixel.NeoPixel(LED_PIN, NUM_LEDS, brightness=AMPLITUDE, auto_write=False, pixel_order=ORDER)

# History of activated LED indices
history = collections.deque(maxlen=MAX_HISTORY)

# Initialize UDP Socket
udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
udp_socket.bind((HOST, PORT))
print(f"UDP server listening on {HOST}:{PORT}")

# Helper to compute order parameter
def compute_order_parameter(history):
    if not history:
        return 0.0
    flat = [i for batch in history for i in batch]
    if len(flat) <= 1:
        return 0.0
    stddev = statistics.stdev(flat)
    normalized_std = stddev / NUM_LEDS
    order = 1.0 - normalized_std
    return min(max(order, 0.0), 1.0)

# Main loop
while True:
    data, client_address = udp_socket.recvfrom(1024)
    start = time.time()
    decoded_data = [int(data.decode('utf-8'))]
    print(f"Received message from {client_address}: {decoded_data}")

    history.append(decoded_data)
    order_param = compute_order_parameter(history)
    print(f"Order parameter: {order_param:.2f}")

    if order_param > 0.8:
        # Too ordered, introduce disorder
        indices = random.sample(range(NUM_LEDS), k=min(5, NUM_LEDS))
    elif order_param < 0.2:
        # Too disordered, create a centered pattern
        center = NUM_LEDS // 2
        indices = [center - 1, center, center + 1]
    else:
        indices = decoded_data

    for i in indices:
        if 0 <= i < NUM_LEDS:
            leds[i] = COLOR

    leds.show()
    udp_socket.sendto(b"done", client_address)
    time.sleep(1.0 / FPS)
    leds.fill((0,0,0))
    leds.show()
