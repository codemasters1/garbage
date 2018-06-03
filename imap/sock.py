import socks

s = socks.socksocket()

s.set_proxy(socks.SOCKS5, 'localhost', 9050)

s.connect(("wp.pl", 80))
s.sendall(b"GET / HTTP/1.1\r\nHost: wp.pl\r\n\r\n")

while True:
	print(s.recv(4096))