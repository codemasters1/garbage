import asyncio
import websockets
import serial
import json
import struct

s = serial.Serial('COM24')


def hsv_to_rgb(h, s, v):
    if s == 0.0: return (v, v, v)
    i = int(h*6.) # XXX assume int() truncates!
    f = (h*6.)-i; p,q,t = v*(1.-s), v*(1.-s*f), v*(1.-s*(1.-f)); i%=6
    if i == 0: return (v, t, p)
    if i == 1: return (q, v, p)
    if i == 2: return (p, v, t)
    if i == 3: return (p, q, v)
    if i == 4: return (t, p, v)
    if i == 5: return (v, p, q)


async def hello():
    async with websockets.connect('wss://home.spoder.pl/ws') as websocket:
        while True:
            event, data = json.loads(await websocket.recv())
            if event == 'hue':
                r, g, b = (int(x*255) for x in hsv_to_rgb(data['value'] / 360.0, 1, 1))
                s.write(struct.pack('BBBBB', 0x21, 0x37, r, g, b))
                s.write(struct.pack('BBBBB', 0x21, 0x37, r, g, b))
                s.flush()



asyncio.get_event_loop().run_until_complete(hello())