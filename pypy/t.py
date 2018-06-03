import socket
import sbs1

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(('server', 30003))

while True:
	data = s.recv(2048)
	print('dd', data)
	data = str(data.strip())
	
	message = sbs1.parse_message(data)
	print(message)

s.close()