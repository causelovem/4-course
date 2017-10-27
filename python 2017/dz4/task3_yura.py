

def ch(m):
    if(m[0][0] != 0):
        return (1, 0)
    for i in range(len(m)):
        if (m[i][0] != 0):
            m[i], m[0] = m[0], m[i]
            return (-1, 0)
    return (1, 1)


p = eval(input())
N = len(p)
mas = [list(eval(input())) for x in range(N - 1)]
mas.insert(0, list(p))

nMas = [list(0 for i in range(N - 1)) for i in range(N - 1)]
nM = N - 1

sign = 1
for s in range(N - 1):
    (tmp, err) = ch(mas)
    sign *= tmp
    if(err == 1):
        break
    for i in range(nM):
        for j in range(nM):
            nMas[i][j] = mas[0][0] * mas[i + 1][j + 1] - mas[0][j + 1] * mas[i + 1][0]
    nM -= 1
    sign *= mas[0][0] ** nM
    mas = nMas.copy()
    nMas.clear()
    nMas = [list(0 for i in range(nM)) for i in range(nM)]
print(mas[0][0] // sign)
