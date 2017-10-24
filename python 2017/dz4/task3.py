# DodgsonDet


import itertools


def cnt(m):
    res = 0
    for i in range(len(m)):
        res += m.index(i)
        m.remove(i)
    return res

p = list(eval(input()))
mas = list()
mas.append(p)
for i in range(len(p) - 1):
    mas.append(list(eval(input())))

if(len(p) > 8):
    return 

j = [x for x in range(len(mas))]
g = itertools.permutations(j)
res = 0
for i in g:
    tmp = 1
    for k in range(len(mas)):
        tmp *= mas[k][i[k]]
    res += (-1)**cnt(list(i)) * tmp
print(res)
