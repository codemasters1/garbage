from collections import OrderedDict

def cheat():
    raise Exception('aaaaaaaa xx')

class Dd:
    def __init__(self):
        self.pos = [0, 0]
        self.dir = 0
        self.visited = OrderedDict()
        self.first_visited_twice = None

    def turn(self, d):
        if d == 'L':
            self.dir = (self.dir - 1) % 4
        elif d == 'R':
            self.dir = (self.dir + 1) % 4
        else:
            cheat()

    def go(self, d):
        d = int(d)
        if self.dir == 0:
            self.pos[1] += d
        elif self.dir == 1:
            self.pos[0] += d
        elif self.dir == 2:
            self.pos[1] -= d
        elif self.dir == 3:
            self.pos[0] -= d
        else:
            cheat()

        i = tuple(self.pos)
        self.visited[i] = self.visited.get(i, 0) + 1
        if self.visited[i] == 2 and not self.first_visited_twice:
            self.first_visited_twice = i

        print(self.dir, self.pos)

    def do(self, step):
        d, s = step[0], step[1:]
        self.turn(d)
        self.go(s)

    def dist(self):
        x, y = self.pos
        return abs(x) + abs(y)

dd = Dd()

for step in 'R8, R4, R4, R8'.split(', '):
    dd.do(step)

fvt = dd.first_visited_twice
print(sum(map(abs, fvt)))

print(dd.dist())
