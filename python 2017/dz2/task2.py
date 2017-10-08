# SectionShuffle

vec = []
vec = eval(input())

vec1 = []
vec2 = []

for i in range(0, len(vec)):
    if (i % 2 == 0):
        vec1.append(vec[i])
    else:
        vec2.append(vec[i])

vec1.reverse()
vec1 = vec1 + vec2
print(tuple(vec1))
