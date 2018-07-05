# Gen

import random as rand
import sys
import numpy as np

if len(sys.argv) > 3:
    print("NO")
    sys.exit(0)

rand.seed()

matrix = open(sys.argv[1], "w")

n = int(sys.argv[2])

mat = np.zeros((n, n), dtype="complex")

for i in range(n):
    for j in range(i, n):
        # re = rand.random() * 100
        # im = rand.random() * 100
        re = 0
        im = 0
        if (i == 3 and j == 3):
            re = 1

        mat[i][j] = complex(re, im)
        mat[j][i] = complex(re, im).conjugate()
    mat[i][i] = complex(mat[i][i].real, 0)

for i in range(n):
    for j in range(n):
        matrix.write(str(mat[i][j]) + " ")
    matrix.write("\n")

matrix.close()
