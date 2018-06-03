
def f(n):
	r = 1
	for i in range(2, n + 1):
		r = (r % 10) * (i % 10)

		while r % 10 == 0:
			r = r // 10;
		r %= 10
	return r

print(24, f(24))
