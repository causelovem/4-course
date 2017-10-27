

def direct():
    (v, u) = (1, 0)
    while True:
        yield (v, u)
        if (v, u) == (1, 0):
            (v, u) = (0, 1)
        elif (v, u) == (0, 1):
            (v, u) = (-1, 0)
        elif (v, u) == (-1, 0):
            (v, u) = (0, -1)
        elif (v, u) == (0, -1):
            (v, u) = (1, 0)


(M, N) = eval(input())  # кол-во стобцов, строк

mas = [[-1 for x in range(M + 2)] for y in range(N + 2)]
for i in range(1, N + 1):
    for j in range(1, M + 1):
        mas[i][j] = -2

cnt = 0
gen = direct()
(x, y) = (1, 1)
(v, u) = next(gen)
for i in range(M * N):
    mas[y][x] = cnt
    cnt = (cnt + 1) % 10
    if (mas[y + u][x + v] != -2):
        (v, u) = next(gen)
    x += v
    y += u

for i in range(1, N + 1):
    print(*(mas[i][j] for j in range(1, M + 1)))
