import ssl
import base64
from socket import *


cc = socket(AF_INET, SOCK_STREAM)
cc.connect(('xx.pl', 443))

cc.send(b'helo xx.pl\r\n')
cc.send(b'starttls\r\n')
# cc.read(..) If the server responds ok to starttls
#             tls negotiation needs to happen and all
#             communication is then over the SSL socket

scc = ssl.wrap_socket(cc, ssl_version=ssl.PROTOCOL_TLSv1)
scc.send('auth login\r\n')
# scc.read(..)

scc.send(base64.b64encode('username')+'\r\n')
scc.send(base64.b64encode('password')+'\r\n')

# css.send(
#  mail from:
#  rcpt to:
#  data
#  etc
