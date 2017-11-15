# MostPopular

inp = input()

inp = inp.split()

d = {}

for i in inp:
    d[i.lower()] = 0

for i in inp:
    d[i.lower()] += 1

max = 0
for i in d.keys():
    if d[i] > max:
        max = d[i]

cnt = 0
for i in d.keys():
    if d[i] == max:
        cnt += 1

print(cnt)
