# Quantum

import sys
import numpy as np
import matplotlib.pyplot as plt


# Генерация массива комбинаций
def makeCombination(N):
    C = []
    for i in range(N + 1):
        C.append([])
        for j in range(i + 1):
            if j == 0:
                C[i].append(1)
            elif j == 1:
                C[i].append(i)
            elif j == i:
                C[i].append(1)
            else:
                C[i].append(C[i - 1][j - 1] + C[i - 1][j])

    return C


# Генерация массивов для сегментации по матрицам
def createBlockCoordsMas():
    i = 0
    s = 0
    v = []
    v1 = []

    for l in range(params['k'] + 1):
        v1.append(l)
        v1.append(s)

        tmp = 0
        if params['eMin'] - l > 0:
            tmp = params['eMin'] - l
        for m in range(tmp, params['eMax'] - l + 1):
            if m <= 2 * params['N']:
                v.append(i)
                v.append(C[2 * params['N']][m])
                s += C[2 * params['N']][m]
                i += v[len(v) - 1]
                v.append(m)

    return v, v1


# Получение номера блока,
# и координаты в нем по глобальным координатам
def getCoordsInDiagBlock(i, j):
    for k in range(params['size'] - 3, -1, -3):
        if (v[k] <= i) and (v[k] <= j) and (v[k] + v[k + 1] - 1 >= i) and (v[k] + v[k + 1] - 1 >= j):
            return v[k + 2], i - v[k], j - v[k]

    return -1, -1, -1


# Генерация вектора-состояния
# по номеру блока и координате
def buildState(nBlock, coord):
    k = nBlock
    n = params['N'] * 2
    res = []

    while n > 0:
        if n == k:
            res.append(1)
            k -= 1
            n -= 1
            continue

        if k == 0:
            res.append(0)
            n -= 1
            continue

        if coord >= C[n - 1][k]:
            res.append(1)
            coord -= C[n - 1][k]
            k -= 1
        else:
            res.append(0)

        n -= 1

    return res


# Вычисление перехода по двум состояниям
def calcTransfer(st1, st2):
    tmp = []

    i1 = -1
    i2 = -1
    cnt = 0

    res = 0.0

    for i in range(len(st1)):
        tmp.append(bool(st1[i]) ^ bool(st2[i]))

    for i in range(len(tmp)):
        if tmp[i] == 1:
            if i1 == -1:
                i1 = i
            else:
                i2 = i
            cnt += 1

    if cnt == 0:
        for i in range(len(st1)):
            if (i % 2 == 0) and (st1[i] == 1):
                res += params['wa']
            elif (i % 2 == 1) and (st1[i] == 1):
                res += params['wb']
    elif cnt == 2:
        if (st1[i1] + st1[i2] == 1) and (st2[i1] + st2[i2] == 1):
            if (i1 % 2 == 1) and (i2 - 2 == i1):
                # фотоны в соседних полостях
                res = params['b']
            elif (i1 % 2 == 0) and (i2 - 1 == i1):
                # атом и фотон в одной полости
                res = params['a']

    return res


# Генерация матрицы H
def makeH():
    H = np.zeros((params['matrixDim'], params['matrixDim']), dtype="complex")
    for i in range(params['matrixDim']):
        for j in range(params['matrixDim']):
            nBlock, i1, j1 = getCoordsInDiagBlock(i, j)
            if nBlock == -1:
                H[i, j] = 0
            else:
                st1 = buildState(nBlock, i1)
                st2 = buildState(nBlock, j1)

                H[i, j] = calcTransfer(st1, st2)

    return H


# Генерация матриц L0, L1
def makeL0L1():
    l0 = np.zeros((params['matrixDim'], params['matrixDim']), dtype="complex")
    l1 = np.zeros((params['matrixDim'], params['matrixDim']), dtype="complex")

    for i in range(params['matrixDim']):
        for j in range(params['matrixDim']):
            for l in range(params['matrixDim']):
                jTmp = 0
                nBlock1, i1, jTmp = getCoordsInDiagBlock(i, l)
                if nBlock1 != -1:
                    break

            for l in range(params['matrixDim']):
                iTmp = 0
                nBlock2, iTmp, j1 = getCoordsInDiagBlock(l, j)
                if nBlock2 != -1:
                    break

            k1 = 0
            k2 = 0
            for k in range(len(v1) - 1, -1, -2):
                if i >= v1[k]:
                    k1 = v1[k - 1]
                    break

            for k in range(len(v1) - 1, -1, -2):
                if j >= v1[k]:
                    k2 = v1[k - 1]
                    break

            st1 = buildState(nBlock1, i1)
            st2 = buildState(nBlock2, j1)

            iTmp = 0
            cnt = 0
            for k in range(len(st1)):
                if bool(st1[k]) ^ bool(st2[k]) == 1:
                    iTmp = k
                    cnt += 1

            if cnt == 1:
                if (iTmp == 1) and (st1[iTmp] == 0) and (st2[iTmp] == 1) and (k1 == k2):
                    l0[j][i] = 1
                    l1[j][i] = 0
                    continue
                elif (iTmp == len(st1) - 2) and (st1[iTmp] == 1) and (st2[iTmp] == 0) and (k2 - k1 == 1):
                    l0[j][i] = 0
                    l1[j][i] = 1
                    continue

            l0[j][i] = 0
            l1[j][i] = 0

    return l0, l1


# Генерация матриц Di
def makeD(num):
    D = np.zeros((params['matrixDim'], params['matrixDim']), dtype="complex")

    for i in range(params['matrixDim']):
        nBlock1, i1, j1 = getCoordsInDiagBlock(i, i)
        st = buildState(nBlock1, i1)
        if st[num * 2] == 1:
            D[i][i] = 1

    return D


# Суммирование диагональных элементов для разного значения стока
def makeRval(res):
    Rv = []
    for i in range(1, len(v1) - 1, 2):
        su = 0
        for j in range(v1[i], v1[i + 2]):
            su += res[j, j]
        Rv.append(su.real)

    return Rv


def calcE(Rv):
    E = 0
    for i in range(0, len(Rv)):
        E += i * Rv[i]

    return E


paramsFile = open("params", "r")

params = {}

# Считывание и парсинг параметров
# k == b, mu == a
# b - photon, a = atom
for s in paramsFile:
    s = s.split()
    if s[0] == "R-file":
        params[s[0]] = s[1]
    elif s[2] == "int":
        params[s[0]] = int(s[1])
    else:
        params[s[0]] = float(s[1])

paramsFile.close()

C = makeCombination(2 * params['N'])

# Создание векторов для сгментации по матрице H
v, v1 = createBlockCoordsMas()

params['size'] = len(v)
params['matrixDim'] = v[len(v) - 3] + v[len(v) - 2]

# Проверка: если есть аргумент командной строки,
# то программа выведет размер матрицы плотности
# который нужно сгенерирвать для данных параметров
if len(sys.argv) >= 2:
    # print(getCoordsInDiagBlock(3, 3))
    # print(buildState(1, 2))
    print("Size = ", params['matrixDim'])
    sys.exit(0)


# Генерация матриц H, L0, L1, Di
H = makeH()
l0, l1 = makeL0L1()

D = [np.matrix(makeD(i)) for i in range(params['N'])]


# Считывание матрицы плотности
Rfile = open(params['R-file'], "r")
R = np.zeros((params['matrixDim'], params['matrixDim']), dtype="complex")

i = 0
for line in Rfile:
    line = line.split()
    for j in range(params['matrixDim']):
        R[i][j] = complex(line[j])
    i += 1

Rfile.close()

H = np.matrix(H)
R = np.matrix(R)
l0 = np.matrix(l0)
l1 = np.matrix(l1)

# Нахождение СВ и СЗ для H
eigenvalue, eigenvectors = np.linalg.eigh(H)

eigenvectors = np.matrix(eigenvectors)

# exp(H)
eigV = np.zeros((params['matrixDim'], params['matrixDim']), dtype="complex")
const = complex(0, - params['dt'] / params['HPLANKS'])
for i in range(params['matrixDim']):
    eigV[i][i] = np.exp(complex(eigenvalue[i], 0) * const)
eigenvalue = np.matrix(eigV)

Z = eigenvectors * eigenvalue * eigenvectors.H

d = params['d']

v1.append(v1[len(v1) - 2] + 1)
v1.append(params['matrixDim'])

# Заочное перемножение матриц, для того, чтобы снизить
# количество перемножений в цикле
L = [l0.H * l0, l1.H * l1]
Dd = [D[i].H * D[i] for i in range(params['N'])]

xcoord = [params['dt'] * i for i in range(params['n'])]
ycoord = [0 for i in range(params['n'])]

copyR = R.copy()
for k in range(params['graphs']):
    res = copyR.copy()
    for i in range(params['n']):
        R = res.copy()

        # Унитарная эволюция
        res = Z * R * Z.H

        # Приток, сток
        S0 = l0 * R * l0.H - 0.5 * (L[0] * R + R * L[0])
        S1 = l1 * R * l1.H - 0.5 * (L[1] * R + R * L[1])

        S0 *= params['lIn'] / params['HPLANKS']
        S1 *= params['lOut'] / params['HPLANKS']

        S = params['dt'] * (S0 + S1)

        res += S

        # Дефазировка
        Ds = np.matrix(
            np.zeros((params['matrixDim'], params['matrixDim']), dtype="complex"))
        for j in range(params['N']):
            Ds += d * (Dd[j] * R * Dd[j] - 0.5 * (Dd[j] * R + R * Dd[j]))
        Ds *= params['dt']

        res += Ds

        Rval = makeRval(res)
        ycoord[i] = calcE(Rval)

    plt.plot(xcoord, ycoord, label='d = ' + str(d))
    print('d = ', d)

    # Суммирование диагональных элементов для разного значения стока
    # (получается случайная величина)
    Rval = makeRval(res)
    E = calcE(Rval)
    print(Rval, E)
    d += params['dStep']

plt.xlabel('time')
plt.ylabel('sink')
plt.legend()
plt.show()

# fH = open("ha", "w")

# for i in range(params['matrixDim']):
#     for j in range(params['matrixDim']):
#         fH.write(str(H[i, j].real) + " ")
#     fH.write("\n")

# fH.close()

# for k in range(len(D)):
#     fD = open("d" + str(k), "w")
#     for i in range(params['matrixDim']):
#         for j in range(params['matrixDim']):
#             fD.write(str(D[k][i, j].real) + " ")
#         fD.write("\n")
#     fD.close()


# print(res)
# print(H)
# print("\n")
# print(l0)
# print("\n")
# print(l1)
# print("\n")
# print(R)
# print("\n")
# print(D)
# print("\n")
# print(eigenvalue)
# print("\n")
# print(eigenvectors)
