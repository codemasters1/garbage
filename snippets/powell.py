def find_coefficients(qa, qb, qc, t):
    return qa, (4*qb - qc - 3*qa) / (2 * t), (qa - 2*qb + qc) / (2 * t * t)

def interpolate(a, b, c):
    return lambda x: a + b * x + c * x**2

def powell(q, e):
    tk = 0
    t = 1

    for k in range(1000):
        tkp = tk + t
        qtk = q(tk)
        qtkp = q(tkp)

        if qtkp > qtk:
            t *= -1
        if qtkp <= qtk:
            t *= 2
            continue
        if qtkp > qtk and k >= 1:
            t *= 0.5
            qa = qtk
            qb = q(tkp - t)
            qc = qtkp

            a, b, c = find_coefficients(qa, qb, qc, t)
            f = interpolate(a, b, c)

            t_star = -b / (2 * c)
            print(t_star)
            if abs((f(t_star) - q(t_star)) / q(t_star)) <= e:
                return t_star

            t *= 0.5
            tk = tkp

if __name__ == '__main__':
    print(powell(lambda x: (x-1)**2, 0.001))
