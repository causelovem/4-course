# PaidStairs

mas = eval(input())
f = []
f.append(mas[0])
f.append(mas[1])
i = 2
while i < len(mas):
    f.append(min(f[i - 1], f[i - 2]) + mas[i])
    i += 1

print(f[len(mas) - 1])
