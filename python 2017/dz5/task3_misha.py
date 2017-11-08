def comp(a, b):
    if len(a) != len(b):
        return 0

    for i in range(len(a)):
        if not((b[i] == a[i]) or (b[i] == "@")):
            return 0
    return 1


inp = input()
mask = input()

res = -1

for i in range(len(inp)):
    if ((mask[0] == inp[i]) or (mask[0] == "@")) and (comp(inp[i:len(mask) + i], mask)):
        res = inp.find(inp[i:len(mask) + i])
        break

print(res)
