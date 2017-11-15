inp = input().split()
max = 0;
s = {}
for i in inp:
	if i.lower() in s:
		s[i.lower()] += 1
		if max < s[i.lower()]:
			max = s[i.lower()]
	else:
		s[i.lower()] = 0

cnt = 0
for k in s.keys():
	if s[k] == max:
		cnt += 1

print(cnt)
