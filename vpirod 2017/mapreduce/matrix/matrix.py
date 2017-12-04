#!/usr/bin/env python
import mincemeat
import csv

fileM1 = open("mat1.csv", 'r')
fileM2 = open("mat2.csv", 'r')


first = 0
datasource = {}

# parcer
mat1 = csv.reader(fileM1)
mat2 = csv.reader(fileM2)

M = iter(mat1)
N = iter(mat2)

# header
next(M)
next(N)

# sizes
sizeM = next(M)
sizeN = next(N)

dictM = {(0, i, j): 0 for i in range(sizeM[0]) for j in range(sizeM[1])}
dictN = {(1, i, j): 0 for i in range(sizeN[0]) for j in range(sizeN[1])}

# dict((type, i, j), value)
for cord in M:
    dictM[(0, cord[0], cord[1])] = cord[2]

for cord in N:
    dictN[(1, cord[0], cord[1])] = cord[2]


def mapfn(k, v):
    for i in v:
        yield k, i


def reducefn(k, vs):
    result = set()

    for i in vs:
        result = result | set([i])
    return len(result)


s = mincemeat.Server()
s.datasource = datasource
s.mapfn = mapfn
s.reducefn = reducefn

results = s.run_server(password="changeme")

fileRes = open("fileRes.csv", 'w')

fileRes.write("Character,number of unique words\n")
for k in results.keys():  # out
    fileRes.write(k + ',' + str(results[k]) + "\n")

fileIn.close()
fileRes.close()
