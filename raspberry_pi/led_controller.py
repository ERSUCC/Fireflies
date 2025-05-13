import socket
import time
import board
import neopixel
import json

# Socket Parameters
HOST = "0.0.0.0"
PORT = 8080

# LED Parameters
NUM_LEDS = 50
LED_PIN = board.D18
ORDER = neopixel.RGB
AMPLITUDE = 0.009
COLOR = (255,200,0)
MIRRORED_LEDS = [4, 7, 15, 20, 31]

# Initializing LEDs
leds = neopixel.NeoPixel(LED_PIN, NUM_LEDS, brightness=AMPLITUDE, auto_write=False, pixel_order=ORDER)

# Initializing UDP Socket
udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
udp_socket.bind((HOST, PORT))
print(f"UDP server listening on {HOST}:{PORT}")

# Core logic
while True:
    data, client_address = udp_socket.recvfrom(1024)
    start = time.time()
    decoded_data = [ int(data.decode('utf-8')) ]
    print(f"Received message from {client_address}: {decoded_data}")

    for i in range(len(decoded_data)):
        leds[decoded_data[i]] = (COLOR)

    leds.show()
    response = "done"
    udp_socket.sendto(response.encode(), client_address)
    print((time.time() - start) * 1000)
    time.sleep(0.05)
    leds.fill((0,0,0))
    leds.show()

    print((time.time() - start) * 1000)
