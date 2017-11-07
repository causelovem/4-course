import sys

def myCmp(a, b):
	if len(a) != len(b):
		return False
	for i in range(0, len(a)):
		if not((b[i] == a[i]) or (b[i] == "@")):
			return False
	return True

qwe = input()
S2 = input()

for i in range(len(qwe)):
	if (S2[0] == "@") or (S2[0] == qwe[i]):
		if myCmp(qwe[i:len(S2)+i], S2):
			print(qwe.find(qwe[i:len(S2)+i]))
			sys.exit()
print(-1)
