import socket
import time
import random

# Socket Parameters
HOST = "10.0.0.42"
PORT = 8080

# Socket Initialization
client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Parameters
TOTAL_FIREFLIES = 10
TOTAL_LEDS = 50

# Create random mapping
firefly_ids = list(range(TOTAL_FIREFLIES))
led_ids = list(range(TOTAL_LEDS))

random.shuffle(led_ids)

if TOTAL_FIREFLIES <= TOTAL_LEDS:
    mapping = dict(zip(firefly_ids, led_ids[:TOTAL_FIREFLIES]))
else:
    mapping = {firefly: led_ids[i % TOTAL_LEDS] for i, firefly in enumerate(firefly_ids)}

print(mapping)

# Event-Based Messaging System

while (True):

    input_firefly_ids = [i for i in range(10)]
    input_led_ids = [mapping[firefly_id] for firefly_id in input_firefly_ids]

    print(f"Current simulation input: {input_firefly_ids}")
    print(f"LED output indices: {input_led_ids}")

    while True:
        start_time = time.time()

        text = input()

        encoded_message = text.encode('utf-8')
        client_socket.sendto(encoded_message, (HOST, PORT))

        # Receive response
        data, server_address = client_socket.recvfrom(1024)
        end_time = time.time()

        # Print response and latency
        print(f"Response from server: {data.decode()}")
        latency = (end_time - start_time) * 1000  # Convert to milliseconds
        print(f"Round-trip latency: {latency:.2f} ms")

    # time.sleep(1)

client_socket.close()
