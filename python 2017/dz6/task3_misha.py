# DungeonMap

flag = 0
d = {}
ent = ''
exi = ''
cnt = 0

while flag < 2:
    inp = input()
    tmp = inp.split()
    if len(tmp) == 1:
        flag += 1
    if flag == 0:
        if d.get(tmp[0]) is None:
            d[tmp[0]] = [tmp[1]]
            cnt += 1
        if d.get(tmp[1]) is None:
            d[tmp[1]] = [tmp[0]]
            cnt += 1
        if (tmp[0] != tmp[1]) and (d[tmp[0]] != [tmp[1]]):
            d[tmp[0]] += [(tmp[1])]
            cnt += 1
        if (tmp[0] != tmp[1]) and (d[tmp[1]] != [tmp[0]]):
            d[tmp[1]] += [tmp[0]]
            cnt += 1
    elif flag == 1:
        ent = tmp[0]
    else:
        exi = tmp[0]

if d.get(ent) is None:
    print('NO')
else:
    for i in range(cnt):
        if ent == exi:
            print("YES")
            break
        tmp = ent
        ent = d[tmp][0]
        if (len(d[tmp]) != 1):
            tmp1 = d[tmp][0]
            d[tmp].remove(d[tmp][0])
            d[tmp] += [tmp1]
    else:
        print("NO")
