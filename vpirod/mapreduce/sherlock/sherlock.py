#!/usr/bin/env python
import mincemeat
import nltk
import os

files = os.listdir("./data")

files = sorted(files)

tokenizer = nltk.tokenize.RegexpTokenizer(r'\w+')

datasource = {}

for file in files:
    fileIn = open("./data/" + file, "r")

    line = fileIn.read()

    line = tokenizer.tokenize(line.lower())

    for li in line:
        if datasource.get(li) is None:
            datasource[li] = [file]
        else:
            datasource[li] += [file]

    fileIn.close()


def mapfn(k, v):
    for i in v:
        yield k, i


def reducefn(k, vs):
    files = os.listdir("./data")
    files = sorted(files)

    result = {file: 0 for file in files}
    for i in vs:
        if result.get(i) is None:
            result[i] = 1
        else:
            result[i] += 1

    return result.values()


s = mincemeat.Server()
s.datasource = datasource
s.mapfn = mapfn
s.reducefn = reducefn

results = s.run_server(password="changeme")

fileRes = open("fileRes.csv", 'w')

fileRes.write("Words")

for i in files:
    fileRes.write("," + i)
fileRes.write("\n")

for item in results.items():
    fileRes.write(item[0])
    for j in item[1]:
        fileRes.write("," + str(j))
    fileRes.write("\n")

fileRes.close()
