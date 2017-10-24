#  LookSay


def gen():
    res = [1]
    prev = [1]
    while 1:
        new = []
        i = 0
        while i < len(prev):
            cnt = 1
            num = prev[i]
            i += 1
            while i < len(prev) and prev[i] == prev[i - 1]:
                cnt += 1
                i += 1
            new.append(cnt)
            new.append(num)
        prev = new
        res += new
        yield res


N = eval(input())
g = gen()
res = next(g)
while len(res) <= N:
    res = next(g)

print(res[N])
