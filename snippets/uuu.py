import itertools


def w1(x1, x2, x3, x4, x5):
    return -x1+3*x2-5*x3-x4+4*x5 <= -2

def w2(x1, x2, x3, x4, x5):
    return 2*x1-6*x2+3*x3+2*x4-2*x5 <= 0

def w3(x1, x2, x3, x4, x5):
    return x2-2*x3+x4+x5 <= -5

def bin_iter(n):
    for i in range(2 ** n):
        yield tuple(map(int, list(str(bin(i))[2:].zfill(n))))

for x in bin_iter(5):
    if w1(*x) and w2(*x) and w3(*x):
        print(x)
