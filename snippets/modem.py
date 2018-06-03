from collections import defaultdict
import functools
import serial
import time
import asyncio


class GsmModem:
	def __init__(self, port, baudrate=9600):
		self.__serial = serial.Serial(port, baudrate, timeout=0)
		self.__handlers = defaultdict(list)

	async def send(self, command):
		self.__serial.write((command + '\n').encode('ascii'))
		print('send')
		await asyncio.sleep(1)

	async def loop(self):
		while True:
			line = self.__serial.readline().decode('ascii').strip()
			print(line)
			if line and line != 'OK':
				if line.startswith('+'):
					command, _ = line[1:].split(':', 1)
					for handler in self.__handlers[command]:
						print('handler!')
						await handler(line)
				else:
					print(line)

			await asyncio.sleep(0)

	def handle(self, command):
		def decorator(fn):
			self.__handlers[command.upper()].append(fn)
			return fn
		return decorator

gsm = GsmModem('COM16', 9600)

@gsm.handle('CSQ')
async def csq(data):
	print(data)
	await asyncio.sleep(0)


async def app():
	while True:
		await gsm.send('CSQ')
		await asyncio.sleep(1)


loop = asyncio.get_event_loop()
loop.create_task(gsm.loop())
loop.run_until_complete(app())
loop.close()
