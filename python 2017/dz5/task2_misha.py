def isInt(s):
    try:
        int(s)
        return 1
    except ValueError:
        return 0


inp = input()
max = -float('Inf')

while len(inp) != 0:
    inp.replace('\t', ' ')
    s = inp.split(' ')
    for i in s:
        if ((isInt(i)) and (int(i) > max)):
            max = int(i)
    inp = input()

print(max)
