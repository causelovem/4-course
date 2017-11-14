# ThreeSquares

import math

inp = set(eval(input()))

m = max(inp)
s = set()
le = len(set(inp))

M = math.trunc(math.sqrt(m))
for i in range(1, M + 1):
    Mi = math.trunc(math.sqrt(m - i ** 2))
    for j in range(i, Mi + 1):
        Mij = math.trunc(math.sqrt(m - i ** 2 - j ** 2))
        for k in range(j, Mij + 1):
            s.add((i ** 2 + j ** 2 + k ** 2))

print(len(set(inp) & s))
