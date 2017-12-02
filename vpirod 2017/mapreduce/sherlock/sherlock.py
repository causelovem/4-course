#!/usr/bin/env python
import mincemeat

import nltk
import os

files = os.listdir("./data")
# files = os.listdir("./testdir")

files = sorted(files)

tokenizer = nltk.tokenize.RegexpTokenizer(r'\w+')

first = 0
data = []


# fileIn = open("test", "r")

# line = fileIn.read()

# line = tokenizer.tokenize(line.lower())
# data.append([line, "test"])

# fileIn.close()


for file in files:
    fileIn = open("./data/" + file, "r")
    # fileIn = open("./testdir/" + file, "r")

    line = fileIn.read()

    line = tokenizer.tokenize(line.lower())
    data.append([line, file])

    fileIn.close()


datasource = {}  # dict for mapreduce
k = -1
for i in data:
    for j in i[0]:
        k += 1
        datasource[k] = j + " " + i[1]


def mapfn(k, v):
    yield v, 1


def reducefn(k, vs):
    result = sum(vs)
    return result


s = mincemeat.Server()
s.datasource = datasource
s.mapfn = mapfn
s.reducefn = reducefn

results = s.run_server(password="changeme")

res = []
for item in results.items():
    res.append([item[0], item[1]])

res = sorted(res)


matrix = []

word = ""
j = -1
for i in res:
    tmp = i[0].split()
    if word != tmp[0]:
        word = tmp[0]
        j += 1
        li = [0 for k in files]
        li.append(0)
        li[0] = word
        matrix.append(li)  # new str in matrix

    idx = files.index(tmp[1])  # file number
    matrix[j][idx + 1] = i[1]


fileRes = open("fileRes.csv", 'w')

fileRes.write("Words,")

for i in files:
    fileRes.write(i + ",")
fileRes.write("\n")

for i in matrix:
    for j in i:
        fileRes.write(str(j) + ",")
    fileRes.write("\n")

fileRes.close()
