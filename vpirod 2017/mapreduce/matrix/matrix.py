#!/usr/bin/env python
import mincemeat
import csv

fileIn = open("matrix.csv", 'r')

datasource = {}

# parcer
matrix = csv.reader(fileIn)

mat = iter(matrix)

# header
next(mat)

# sizes
size = next(mat)  # i j j k

for i in mat:
    # (type, i, j, j/k): (value)
    if i[0] == "a":
        datasource[("a", int(i[1]), int(i[2]), int(size[3]))] = int(i[3])
    else:
        datasource[("b", int(i[1]), int(i[2]), int(size[0]))] = int(i[3])


def mapfn(k, v):
    # (i, k): (type, j, value)
    if k[0] == "a":
        for i in range(k[3]):
            yield (k[1], i), (k[0], k[2], v)
    else:
        for i in range(k[3]):
            yield (i, k[1]), (k[0], k[2], v)


def reducefn(k, vs):
    mat1 = []
    mat2 = []

    for i in vs:
        if i[0] == "a":
            mat1.append(i)
        else:
            mat2.append(i)

    mat1 = sorted(mat1, key=lambda item: item[1])
    mat2 = sorted(mat2, key=lambda item: item[1])

    m1 = len(mat1)  # M
    m2 = len(mat2)  # N
    cnt = 0
    result = 0
    for i in range(m1):
        for j in range(cnt, m2):
            if mat1[i][1] == mat2[j][1]:
                cnt = j + 1
                result = (result + (mat1[i][2] * mat2[j][2]) % 97) % 97
                # result += mat1[i][2] * mat2[j][2]
                break

    return result


s = mincemeat.Server()
s.datasource = datasource
s.mapfn = mapfn
s.reducefn = reducefn

results = s.run_server(password="changeme")

fileRes = open("fileRes.csv", 'w')

fileRes.write("i,j,value\n")
fileRes.write(str(size[0]) + "," + str(size[1]) + "," + str(size[2]) + "," + str(size[3]))

for k in results.keys():  # out
    if results[k] != 0:
        fileRes.write("\n" + str(k[0]) + ',' + str(k[1]) + ',' + str(results[k]))

fileIn.close()
fileRes.close()
