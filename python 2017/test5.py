# AnyPower

a = eval(input())
border = int(a ** (1 / 2)) + 1

for x in range(0, border):
	while x < a:
		x *= x
	if x == a:
		break
	