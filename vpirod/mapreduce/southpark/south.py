#!/usr/bin/env python
import mincemeat
import nltk
import csv

fileIn = open("All-seasons.csv", 'r')

tokenizer = nltk.tokenize.RegexpTokenizer(r'\w+')

datasource = {}

# parcer
reader = csv.reader(fileIn)

it = iter(reader)
next(it)

# dict(name, [speech])
for line in it:
    name = line[2].replace(" and ", ", ").split(", ")
    speech = tokenizer.tokenize(line[3].lower())

    for na in name:
        if datasource.get(na) is None:
            datasource[na] = speech
        else:
            datasource[na] += speech


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
