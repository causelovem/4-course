# SecondMax

vec = eval(input())

max1 = vec[0]
max2 = vec[0]
for i in range(1, len(vec)):
    if (vec[i] > max1):
        max1 = vec[i]
    if (vec[i] < max2):
        max2 = vec[i]

for i in range(0, len(vec)):
    if ((vec[i] > max2) and (vec[i] != max1)):
        max2 = vec[i]

if max1 == max2:
    print('NO')
else:
    print(max2)
