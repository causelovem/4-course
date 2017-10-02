# IntPalindrome
a = eval(input())
c = int(1)
while c < a:
    c *= 10
c = int(c / 10)
result = 'YES'

while a > 10:
    first = int(a / c)
    last = a % 10
    a -= first * c
    a = int(a / 10)
    c = int(c / 100)
    # print(first, ' ', last)
    if (first != last):
        result = 'NO'
        break

print(result)
