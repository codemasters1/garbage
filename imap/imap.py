import asyncio
import socks

class ImapClientProtocol(asyncio.Protocol):
    def __init__(self):
        self._transport = None
        self._buffer = bytearray()

        self._auth_tag = None

        self._wait_logged = asyncio.Future()

    def connection_made(self, transport):
        self._transport = transport

    def data_received(self, data):
        self._buffer += data
        position = self._buffer.find(b'\r\n')

        line = self._buffer[0:position].decode()
        self._buffer = self._buffer[position+2:]

        self._parse_line(line)

    def _generate_tag(self):
        return 'A1'

    def _send_command(self, command, *args):
        tag = self._generate_tag()

        args = ['"' + arg + '"' for arg in args]
        self._transport.write('{0} {1} {2}\r\n'.format(tag, command, ' '.join(args)).encode('utf-8'))

        return tag

    @asyncio.coroutine
    def login(self, login, password):
        self._auth_tag = self._send_command('LOGIN', login, password)

        yield from self._wait_logged

        return self._wait_logged.result()

    @asyncio.coroutine
    def logout(self):
        self._send_command('LOGOUT')

    @property
    def auth_tag(self):
        return self._auth_tag

    def _parse_line(self, line):
        tag, response = line.split(' ', 1)

        print(tag, response)

        if tag == self.auth_tag:
            self._wait_logged.set_result('OK' in response)

    def connection_lost(self, exc):
        self.waiter.set_result(None)


@asyncio.coroutine
def check_account(loop, login, password, host, port, ssl=False):
    socket = socks.socksocket()
    socket.set_proxy(socks.SOCKS5, 'localhost', 9050)
    socket.connect((host, port))

    transport, protocol = yield from loop.create_connection(ImapClientProtocol, ssl=ssl, sock=socket)
    response = yield from protocol.login(login, password)
    yield from protocol.logout()

    return response

loop = asyncio.get_event_loop()
client = check_account(loop, 'xx', 'xx', 'xx', 143)
a = loop.run_until_complete(client)

print(a)