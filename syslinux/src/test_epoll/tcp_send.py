import socket
from time import sleep

socks = []
for i in range(5):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM, socket.IPPROTO_TCP);
    sock.connect(("127.0.0.1", 8190))
    socks.append(sock)

count = 5

while count:
    for i in range(5):
        print "send data"
        socks[i].sendall("hello %d" % i)
        sleep(1)
    count -= 1
        
for sock in socks:
    sock.close()
