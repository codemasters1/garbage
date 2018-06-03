def fmin(f):
    step = 1
    tprev = 0

    for k in range(10):
        t = tprev + step

        if f(t) <= f(tprev):
            step *= 2.0
        else:
            if k == 0:
                step *= -1.0
            else:
                step *= 0.5

        print(tprev, 0, step, 2*step)
        tprev = t

fmin(lambda x: (x-1)**2)
