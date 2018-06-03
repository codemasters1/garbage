def cheat():
    raise Exception('xxx')

class Dd:
    def __init__(self):
        self.pos = [0, 0]
        self.dir = 0

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

        print(self.dir, self.pos)

    def do(self, step):
        d, s = step[0], step[1:]
        self.turn(d)
        self.go(s)

    def dist(self):
        x, y = self.pos
        return abs(x) + abs(y)

dd = Dd()

for step in 'R3, R1, R4, L4, R3, R1, R1, L3, L5, L5, L3, R1, R4, L2, L1, R3, L3, R2, R1, R1, L5, L2, L1, R2, L4, R1, L2, L4, R2, R2, L2, L4, L3, R1, R4, R3, L1, R1, L5, R4, L2, R185, L2, R4, R49, L3, L4, R5, R1, R1, L1, L1, R2, L1, L4, R4, R5, R4, L3, L5, R1, R71, L1, R1, R186, L5, L2, R5, R4, R1, L5, L2, R3, R2, R5, R5, R4, R1, R4, R2, L1, R4, L1, L4, L5, L4, R4, R5, R1, L2, L4, L1, L5, L3, L5, R2, L5, R4, L4, R3, R3, R1, R4, L1, L2, R2, L1, R4, R2, R2, R5, R2, R5, L1, R1, L4, R5, R4, R2, R4, L5, R3, R2, R5, R3, L3, L5, L4, L3, L2, L2, R3, R2, L1, L1, L5, R1, L3, R3, R4, R5, L3, L5, R1, L3, L5, L5, L2, R1, L3, L1, L3, R4, L1, R3, L2, L2, R3, R3, R4, R4, R1, L4, R1, L5'.split(', '):
    dd.do(step)

print(dd.dist())
