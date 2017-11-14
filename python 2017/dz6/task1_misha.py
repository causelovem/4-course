# ThreeSquares

import math

inp = set(eval(input()))

m = max(inp)
s = set()

M = int(math.sqrt(m))

for i in range(1, int((M + 1))):
    Mi = int(math.sqrt((m - i ** 2)))
    ii = i ** 2
    for j in range(i, int((Mi + 1))):
        jj = j ** 2
        Mij = int(math.sqrt(m - ii - jj))
        for k in range(j, Mij + 1):
            s.add((ii + jj + k ** 2))

print(len(set(inp) & s))
