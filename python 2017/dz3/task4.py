#  GenTriseq


def seqGen(mas):
    res = []
    tmp = 0
    for j in range(len(mas)):
        for i in range(len(mas)):
            if mas[i] <= mas[tmp]:
                res.append(mas[i])
        tmp += 1
        yield res


p = eval(input())
N = eval(input())
g = seqGen(p)
res = []
# res = next(g)
for i in range(len(p)):
    res = next(g)
    if (len(res) > N):
        print(res[N])
        break

if (len(res) < N):
    print('NO')
