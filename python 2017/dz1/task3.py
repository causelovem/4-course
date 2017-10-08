# DotsInCircle
x, y, r = eval(input())
a, b = eval(input())
c = 'YES'
while a or b:
    if ((x - a)**2 + (y - b)**2) > r**2:
        c = 'NO'
        break
    a, b = eval(input())
print(c)
