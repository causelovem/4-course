# FarGalaxy

cnt = 0
d = {}

while 1:
    inp = input()
    tmp = inp.split()
    if len(tmp) == 1:
        break
    cnt += 1
    tmp = [float(tmp[0]), float(tmp[1]), float(tmp[2]), tmp[3]]
    d[cnt] = tmp

dt = {1: 0, 2: '', 3: ''}

for i in range(1, cnt):
    for j in range(i + 1, cnt + 1):
        tmp = (d[i][0] - d[j][0]) ** 2 + (d[i][1] - d[j][1]) ** 2 + (d[i][2] - d[j][2]) ** 2
        if tmp > dt[1]:
            dt[1] = tmp
            dt[2] = d[i][3]
            dt[3] = d[j][3]

if (dt[2] < dt[3]):
    print(dt[2] + ' ' + dt[3])
else:
    print(dt[3] + ' ' + dt[2])
