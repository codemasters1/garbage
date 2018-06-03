valid = 0
with open('input.txt') as f:
    for line in f:
        a, b, c = map(int, filter(None, line.split(' ')))
        if a and b and c and a < b + c and b < a + c and c < a + c:
            valid += 1
        else:
            print(a,b,c)
print(valid)
