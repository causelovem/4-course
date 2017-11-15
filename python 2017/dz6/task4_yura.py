inp = input().split()
mas = []
while len(inp) == 4:
	tmp = []
	tmp.append(float(inp[0]))
	tmp.append(float(inp[1]))
	tmp.append(float(inp[2]))
	tmp.append(inp[3])
	mas.append(tmp)
	inp = input().split()

max = 0

for i in range(len(mas)):
	for j in range(i+1, len(mas)):
		tmp = (mas[i][0] - mas[j][0])**2 + (mas[i][1] - mas[j][1])**2 + (mas[i][2] - mas[j][2])**2
		if tmp > max:
			s1 = mas[i][3]
			s2 = mas[j][3]
			max = tmp

if (s1 < s2):
    print(s1 + ' ' + s2)
else:
    print(s2 + ' ' + s1)
