import socket
from time import sleep

socks = []
for i in range(20):
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM, socket.IPPROTO_TCP);
        sock.connect(("127.0.0.1", 8190))
        socks.append(sock)
       
    except Exception, e:
        print e
    sleep(0.2)

for i in range(20):
    try:
        socks[i].sendall("hello %d" % i)
    except Exception, e:
        print e

for sock in socks:
    sock.close()
