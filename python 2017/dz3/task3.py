#  IterPi


def pigen():
    res = 0
    tmp = -1
    cnt = -1
    while 1:
        tmp += 2
        cnt *= -1
        res += cnt / tmp
        yield res * 4


E = eval(input())
prev_gen = pigen()
prev = next(prev_gen)
j = 1
# for i in next(pigen()):
while 1:
    i = next(prev_gen)
    if abs(prev - i) < E:
        print(j)
        break
    prev = i
    j += 1
