# ParallelSegments

p = eval(input())  # x1 y1 x2 y2 x3 y3 x4 y4

x1 = p[2] - p[0]
x2 = p[6] - p[4]
y1 = p[3] - p[1]
y2 = p[7] - p[5]

s1 = x1 ** 2 + y1 ** 2
s2 = x2 ** 2 + y2 ** 2
sp = (x1 * x2 + y1 * y2) ** 2

if (sp == s1 * s2):
    print('YES')
else:
    print('NO')
