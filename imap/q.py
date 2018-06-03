import asyncio, random

q = asyncio.Queue()


@asyncio.coroutine
def get_line():
    return random.random()

@asyncio.coroutine
def consume(n):
    while True:
        value = yield from get_line()
        print("Consumed{0}".format(n), value)
        yield from asyncio.sleep(0.5 + random.random())


asyncio.Task(consume(1))
asyncio.Task(consume(2))

loop = asyncio.get_event_loop()
loop.run_forever()