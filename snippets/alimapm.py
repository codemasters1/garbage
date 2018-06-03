import numpy as np
from math import inf


def wut(m, lb=0):
    return m, lb


def main():
    a = np.matrix([
        [inf, 302, 132, 340, 285, 290],
        [302, inf, 207, 175, 337, inf],
        [132, 207, inf, 210, 193, inf],
        [340, 175, 210, inf, 179, inf],
        [285, 337, 193, 179, inf, 78],
        [290, inf, inf, inf, 78, inf]
    ])

    n, n = a.shape

    min_in_rows = np.min(a, 1)
    for j in range(n):
        a[j,:] -= min_in_rows[j]

    LB = np.sum(min_in_rows)

    min_in_columns = np.min(a, 0).T
    for i in range(n):
        a[:,i] -= min_in_columns[i]

    LB += np.sum(min_in_columns)

    for j, i in np.ndindex(a.shape):
        if a[j,i] == 0:
            print(j, i)

    print(a, LB)

if __name__ == '__main__':
    main()
