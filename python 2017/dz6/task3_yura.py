inp = input().split()

d = {}
while len(inp) != 1:
	if not(inp[0] in d):
		d[inp[0]] = set()
	if not(inp[1] in d):
		d[inp[1]] = set()
	d[inp[0]].add(inp[1])
	d[inp[1]].add(inp[0])
	
	inp = input().split()

enter = set()
enter.add(inp[0])
exit = input()

src = enter.copy()
while True:
	newS = set()
	for x in src:
		newS = newS | d[x]
	if newS <= enter:
		break
	src = newS - enter
	enter = newS | enter

if exit in enter:
	print('YES')
else:
	print('NO')