import socket
import struct
import matplotlib.pyplot as plt
import numpy as np
import time

SERVER_PORT = 8080
MAX_DATAGRAM_SIZE = 16
MIN_V = 0.0
MAX_V = 3.33
PADDING = 0.5
SAMPLES = 10


def configurate_plots():
    plt.ion()
    figure, axis = plt.subplots(2, 2)
    plots = [axis[0, 0], axis[0, 1], axis[1, 0], axis[1, 1]]
    titles = ["Joystick1 - X", "Joystick1 - Y",
              "Joystick2 - X", "Joystick2 - Y"]
    for plot, title in zip(plots, titles):
        plot.set_ylim([MIN_V, MAX_V + PADDING])
        plot.set_ylabel("Napiecie")
        plot.set_title(title)

    figure.tight_layout(pad=2.0)

    x = np.linspace(0, 10, SAMPLES)
    all_y = [[0] * SAMPLES for _ in range(4)]
    lines_objects = []

    for single_plot, y in zip(plots, all_y):
        temp = single_plot.plot(x, y)[0]
        lines_objects.append(temp)

    return figure, lines_objects, all_y


def update_plots(reading,
                 figure,
                 lines_objects,
                 all_y):
    for values, new_value in zip(all_y, reading):
        del values[0]
        values.append(new_value)
    for line, values in zip(lines_objects, all_y):
        line.set_ydata(values)
    figure.canvas.draw()
    figure.canvas.flush_events()


def create_socket_and_establish_connection() -> socket.socket:
    connection = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    connection.bind(('', 0))
    connection.sendto("test".encode('ascii'), ('10.42.0.63', SERVER_PORT))
    data, size = connection.recvfrom(MAX_DATAGRAM_SIZE)
    print(f"Connected with server: {data.decode('ascii')}")
    connection.setblocking(False)
    return connection


def receive_next_reading(connection: socket.socket):
    while True:
        try:
            last_packet = connection.recv(MAX_DATAGRAM_SIZE)
            return struct.unpack("ffff", last_packet)
        except BlockingIOError:
            time.sleep(0.01)  # nie mamy jeszcze danych
        except socket.error as e:
            print(f"Socket error occurred: {e}")
            raise e


if __name__ == '__main__':
    figure, lines_objects, all_y = configurate_plots()
    connection = create_socket_and_establish_connection()
    while True:
        reading = receive_next_reading(connection)
        print(reading)
        update_plots(reading, figure, lines_objects, all_y)
