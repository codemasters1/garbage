przeloty = [1,2.5,3,4]

def layer_height(i):
    if i == 0:
        return przeloty[i]
    else:
        return przeloty[i] - przeloty[i - 1]

for i in range(len(przeloty)):
    print(layer_height(i))
