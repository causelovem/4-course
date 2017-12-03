import nltk.tokenize


file = open("test", 'r')

tokenizer = nltk.tokenize.RegexpTokenizer(r'\w+')

data = []

first = 0
file.readline()

for line in file:
    # line = file.readline()

    if len(line) == 0:
        break

    if line == '"\n':
        first = 0
        continue

    if first == 0:
        first = 1
        line = line.split(",")
        name = line[2]
        speech = ""

        for i in range(3, len(line)):
            line[i] = line[i].lower()
            speech += line[i]
            speech += " "
    else:
        speech = line

    speech = tokenizer.tokenize(speech)

    data.append([name, speech])

dictData = []
k = -1
for i in data:
    for j in i[1]:
        k += 1
        s = i[0] + " " + j
        dictData.append([k, s])

datasource = dict(dictData)

print(datasource)
file.close()
