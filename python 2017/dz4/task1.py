#  FilterList;


p = list(eval(input()))
(M, N) = eval(input())
mas = []
for i in range(len(p)):
    if (i % M != 0) and (p[i] % N != 0):
        mas.append(p[i])
print(mas)
