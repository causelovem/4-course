# DodgsonDet


# import itertools
import numpy


def changeStr(m, sign):
    if (m[0][0] != 0):
        return (m, sign, 0)

    for i in range(len(m)):
        if (m[i][0] != 0):
            tmp = m[i]
            m[i] = m[0]
            m[0] = tmp
            sign *= -1
            break

    if (m[0][0] == 0):
        return (m, sign, -1)
    return (m, sign, 0)


p = list(eval(input()))
N = len(p) - 1
mas = list()
mas.append(p)
for i in range(N):
    mas.append(list(eval(input())))

print(numpy.linalg.det(numpy.array(mas)))

newMas = list()
for i in range(N):
    newMas.append(list(0 for i in range(N)))

sign = 1
tmp = 0

for k in range(len(p) - 1):
    (mas, sign, tmp) = changeStr(mas, sign)
    if (tmp == -1):
        break
    for i in range(N):
        for j in range(N):
            newMas[i][j] = mas[0][0] * mas[i + 1][j + 1] - mas[0][j + 1] * mas[i + 1][0]
    N -= 1
    sign *= mas[0][0] ** N
    mas = newMas.copy()
    newMas.clear()
    newMas = list()
    for i in range(N):
        newMas.append(list(0 for i in range(N)))

print(int(mas[0][0] / sign))
