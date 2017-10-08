# IntPalindrome
number = eval(input())
tmp = int(1)
while tmp < number:
    tmp *= 10
tmp = int(tmp / 10)
result = 'YES'

while number > 10:
    first = int(number / tmp)
    last = number % 10
    number -= first * tmp
    number = int(number / 10)
    tmp = int(tmp / 100)
    # deleted left and right
    if (first != last):
        result = 'NO'
        break

print(result)
