# CountInt

inp = eval(input())

cnt = 0
for i in dir(inp):
    if str(type(eval('inp.' + i))) == "<class 'int'>":
        cnt += 1

print(cnt)
