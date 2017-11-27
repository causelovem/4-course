inp = eval(input())
res = 0
for i in dir(inp):
    st = "inp." + i
    if str(type(eval(st))) == "<class 'int'>" :
        res += 1

print(res)