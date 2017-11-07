import math

def RepresentsInt(s):
    try: 
        int(s)
        return True
    except ValueError:
        return False

S = input()
maxInt = -float('Inf')

while  len(S) != 0:
	S.replace("\t", " ")
	s = S.split(" ")
	for i in s:
		if RepresentsInt(i) and (int(i) > maxInt):
			maxInt = int(i)
	S = input()
print(maxInt)