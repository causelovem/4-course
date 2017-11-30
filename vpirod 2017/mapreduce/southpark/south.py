#!/usr/bin/env python
import mincemeat

import nltk

fileIn = open("All-seasons.csv", 'r')
# fileIn = open("test", 'r')

tokenizer = nltk.tokenize.RegexpTokenizer(r'\w+')

first = 0
data = []

fileIn.readline()
for line in fileIn:
    if len(line) == 0:
        break

    if line == '"\n':
        first = 0
        continue

    line = line.lower()

    if first == 0:
        first = 1

        li = line.split(",")
        name = li[2]
        speech = line[len(li[0] + li[1] + li[2]) + 3:]
    else:
        speech = line

    speech = tokenizer.tokenize(speech)

    data.append([name, speech])


datasource = {}
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


res = {}

for i in results:
    tmp = i.split()
    tmp = len(tmp[-1]) + 1

    if res.get(i[:-tmp]) is None:
        res[i[:-tmp]] = 1
    else:
        res[i[:-tmp]] += 1


fileRes = open("fileRes.csv", 'w')

fileRes.write("Character, number of unique words\n")
for k in res:
    fileRes.write(k + ', ' + str(res[k]) + "\n")

fileIn.close()
fileRes.close()
