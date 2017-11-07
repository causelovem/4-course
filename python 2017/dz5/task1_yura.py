def divadd(s, d, m, n):
    prev, curr = None, s
    while prev!=curr:
        yield curr
        prev, curr = curr, (curr+n) if curr%d else (curr//d+m)


def catseq(fours):
    for four in fours:
        yield from divadd(*four)


fours = eval(input())
k = eval(input())

for i,e in enumerate(catseq(fours)):
    if i == k:
        print(e)
        break
else:
    print("NO")