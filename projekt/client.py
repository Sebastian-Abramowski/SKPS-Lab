import socket
import struct
import matplotlib.pyplot as plt
import numpy as np

SERVER_PORT = 8080
MAX_DATAGRAM_SIZE = 66507
MIN_V = 0.0
MAX_V = 3.33
SAMPLES = 10


def initialize_plots():
    plt.ion()
    figure, axis = plt.subplots(2, 2)
    all_axis = [axis[0, 0], axis[1, 0], axis[0, 1], axis[1, 1]]
    for number, element in enumerate(all_axis):
        element.set_ylim([MIN_V, MAX_V])
        element.set_ylabel("Napięcie")

    samples = 4 * SAMPLES
    x = np.linspace(0, 10, samples)
    y00 = [0] * samples
    y10 = [0] * samples
    y01 = [0] * samples
    y11 = [0] * samples
    all_y = [y00, y10, y01, y11]
    all_lines = []
    divide = 0
    for _axis, _y in zip(all_axis[:divide], all_y[:divide]):
        temp, = _axis.step(x, _y)
        all_lines.append(temp)
    for _axis, _y in zip(all_axis[divide:], all_y[divide:]):
        temp, = _axis.plot(x, _y)
        all_lines.append(temp)
    return figure, all_lines, all_y


def update_plots(reading, figure, all_lines, all_y):
    for values, new_value in zip(all_y, reading):
        del values[0]
        values.append(new_value)
    for line, values in zip(all_lines, all_y):
        line.set_ydata(values)
    figure.canvas.draw()
    figure.canvas.flush_events()


def create_socket_and_establish_connection() -> socket.socket:
    connection = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    connection.bind(('', 0))
    connection.sendto("".encode('ascii'), ('127.0.0.1', SERVER_PORT))
    data, size = connection.recvfrom(MAX_DATAGRAM_SIZE)
    print(f"Connected with server: {data.decode('ascii')}")
    connection.setblocking(False)
    return connection


def receive_next_reading(connection: socket.socket) -> tuple[float, float, float, float]:
    last_packet = None
    while True:
        try:
            last_packet = connection.recv(MAX_DATAGRAM_SIZE)
        except socket.error as e:
            if e.args[0] == socket.EWOULDBLOCK:
                if last_packet:
                    return struct.unpack("ffff", last_packet)
            else:
                raise e


if __name__ == '__main__':
    figure, all_lines, all_y = initialize_plots()
    connection = create_socket_and_establish_connection()
    print("HALO?")
    while True:
        reading = receive_next_reading(connection)
        print(reading)
        update_plots(reading, figure, all_lines, all_y)
