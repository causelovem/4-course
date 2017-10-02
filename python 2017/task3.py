# DotsInCircle
x, y, r = eval(input())
a, b = eval(input())
c = 'YES'
while a or b:
    if ((a - x)**2 + (b - y)**2) > r**2:
        c = 'NO'
        break
    a, b = eval(input())
print(c)
