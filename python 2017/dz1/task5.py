# AnyPower
import math

a = eval(input())

border = int(math.sqrt(a)) + 1

result = 'NO'
for i in range(2, border):
    c = int(i ** int(math.log(a, i)))
    if c == a:
        result = 'YES'
        break
print(result)
