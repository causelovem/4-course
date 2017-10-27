# SpiralDigits


def gen_num():
    i = -1
    while 1:
        i += 1
        if (i > 9):
            i = 0
        yield i


def gen_dist():
    i = 0
    while 1:
        i += 1
        if (i > 4):
            i = 1
        yield i


M, N = eval(input())
mas = list()
for i in range(N):
    mas.append(list(-1 for j in range(M)))
x = 0
y = 0
dist = gen_dist()
d = next(dist)

gen = gen_num()

for i in range((N) * (M)):
    mas[x][y] = next(gen)

    if (d == 1):
        if ((y == M - 1) or (mas[x][y + 1] != -1)):
            d = next(dist)
    elif (d == 2):
        if ((x == N - 1) or (mas[x + 1][y] != -1)):
            d = next(dist)
    elif (d == 3):
        if ((y == 0) or (mas[x][y - 1] != -1)):
            d = next(dist)
    elif (d == 4):
        if ((x == 0) or (mas[x - 1][y] != -1)):
            d = next(dist)

    if (d == 1):
        y += 1
    elif (d == 2):
        x += 1
    elif (d == 3):
        y -= 1
    elif (d == 4):
        x -= 1

for i in range(N):
    print(*(mas[i][j] for j in range(M)))
