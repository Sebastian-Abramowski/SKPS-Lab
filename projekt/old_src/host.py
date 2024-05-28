import socket

SERVER_IP = '127.0.0.1'  # Zastąp 'Adres_IP_Raspberry_Pi' właściwym adresem IP Raspberry Pi
PORT = 8080                          # Port serwera

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Wysyłanie pierwszego pakietu do serwera, aby ustalić adres klienta


# Nasłuchiwanie odpowiedzi od serwera
sock.bind(('', 0))
sock.sendto(b'Hello, server', (SERVER_IP, PORT))

while True:

    data, addr = sock.recvfrom(1024)
    print('Otrzymano:', data.decode())

