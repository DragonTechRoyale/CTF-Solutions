from pwn import *
import socket
import threading

UDP_IP = "127.0.0.1"
UDP_PORT = 1337

def get_flag():
	sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
	sock.bind((UDP_IP, UDP_PORT))
	data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
	print("flag: %s" % data)

def run_behemoth5():
	process("/behemoth/behemoth5")

def main():
    	get = threading.Thread(target=get_flag)
	run = threading.Thread(target=run_behemoth5)
	get.start()
	run.start()

if __name__ == '__main__':
    main()
