import numpy as np
import math
from functools import reduce
from matplotlib import pyplot
import pylab
from mpl_toolkits.mplot3d import Axes3D
import random

def rotz(theta):
    m = np.eye(4)
    m[0,0] = math.cos(np.radians(theta))
    m[1,1] = m[0,0]
    m[1,0] = math.sin(np.radians(theta))
    m[0,1] = -m[1,0]

    return m

def rotx(alpha):
    m = np.eye(4)
    m[1,1] = math.cos(np.radians(alpha))
    m[2,2] = m[1,1]
    m[2,1] = math.sin(np.radians(alpha))
    m[1,2] = -m[2,1]

    return m

def transz(d):
    m = np.eye(4)
    m[2,3] = d

    return m

def transx(a):
    m = np.eye(4)
    m[0,3] = a

    return m

def czlon(theta, d, a, alpha):
    return reduce(np.dot, [rotz(theta), transz(d), transx(a), rotx(alpha)])

def vis(*t):
    xx = [
        czlon(t[0], 0.4, 0.025, 90),
        czlon(t[1]+90, 0, 0.455, 0),
        czlon(t[2], 0, 0.035, 90),
        czlon(t[3], 0.42, 0, -90),
        czlon(t[4], 0, 0, 90),
        czlon(t[5], 0.08, 0, 0)
    ]

    return reduce(np.dot, xx)


def print_point(p):
    print('{0:1.03f}\t{1:1.03f}\t{2:1.03f}'.format(*p))

def gen():
    ctr = np.array([0,0,0,1])
    for a in range(0, 90, 3):
        for b in range(0, 45, 3):
            for c in range(0, 90, 3):
                u = vis(a,b,c,0, 0 ,0).dot(ctr)
                yield u[0], u[1], u[2]

fig = pylab.figure()
ax = Axes3D(fig)
ax.plot(*zip(*gen()))
pyplot.show()
