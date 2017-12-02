#!/usr/bin/env python
import mincemeat

import nltk
import os

files = os.listdir("./data")

# fileIn = open("All-seasons.csv", 'r')
# fileIn = open("test", 'r')

tokenizer = nltk.tokenize.RegexpTokenizer(r'\w+')

first = 0
data = []

s = "./data/" + files[0]

file = open(s, "r")

for file in files:
    fileIn = open("./data/" + file, "r")

    line = fileIn.read()

    line = tokenizer.tokenize(line.lower())
    data.append([file, line])

    fileIn.close()

# data.remove(data[0])  # deleted headers


datasource = {}  # dict for mapreduce
k = -1
for i in data:
    for j in i[1]:
        k += 1
        datasource[k] = i[0] + " " + j


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


res = {}  # calc

for i in results:
    tmp = i.split()
    tmp = len(tmp[-1]) + 1

    if res.get(i[:-tmp]) is None:
        res[i[:-tmp]] = 1
    else:
        res[i[:-tmp]] += 1


fileRes = open("fileRes.csv", 'w')

fileRes.write("Words, file\n")
for k in res:  # out
    fileRes.write(k + ', ' + str(res[k]) + "\n")

fileRes.close()
