import socket

SERVER_IP = '10.42.0.63'
PORT = 8080

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)



sock.bind(('', 0))
sock.sendto(b'Hello, server', (SERVER_IP, PORT))

while True:

    data, addr = sock.recvfrom(1024)
    print('Otrzymano:', data.decode())

