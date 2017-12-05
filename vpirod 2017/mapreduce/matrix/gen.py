import sys
import random

if len(sys.argv) != 6:
    print("Unexpected number of parameters\n[i, j, j, k, %[0,1]]\n")
    sys.exit(1)

if int(float(sys.argv[5])) != 0:
    print("% must be a float from 0 till 1\n")
    sys.exit(2)

if int(sys.argv[2]) != int(sys.argv[3]):
    print("Number of M.rows must be equal to number of N.cols\n")
    sys.exit(3)


matrixFile = open("matrix.csv", 'w')

matrixFile.write("Type,i,j,value")
matrixFile.write("\n" + sys.argv[1] + "," + sys.argv[2] + "," + sys.argv[3] + "," + sys.argv[4])

random.seed()
mat = []

for i in range(int(sys.argv[1])):
    st = []
    for j in range(int(sys.argv[2])):
        st.append(random.randint(1, 200))
    mat.append(st)

size = int(int(sys.argv[1]) * int(sys.argv[2]) * float(sys.argv[5]))

coords = set()

while len(coords) != size:
    i = random.randint(0, int(sys.argv[1]) - 1)
    j = random.randint(0, int(sys.argv[2]) - 1)

    if (i, j) in coords:
        continue

    coords.add((i, j))
    mat[i][j] = 0


for i in range(int(sys.argv[1])):
    for j in range(int(sys.argv[2])):
        if mat[i][j] != 0:
            matrixFile.write("\n" + "a" + "," + str(i) + "," + str(j) + "," + str(mat[i][j]))


mat = []

for i in range(int(sys.argv[3])):
    st = []
    for j in range(int(sys.argv[4])):
        st.append(random.randint(1, 200))
    mat.append(st)

size = int(int(sys.argv[3]) * int(sys.argv[4]) * float(sys.argv[5]))

coords = set()

while len(coords) != size:
    i = random.randint(0, int(sys.argv[3]) - 1)
    j = random.randint(0, int(sys.argv[4]) - 1)

    if (i, j) in coords:
        continue

    coords.add((i, j))
    mat[i][j] = 0

for i in range(int(sys.argv[1])):
    for j in range(int(sys.argv[2])):
        if mat[i][j] != 0:
            matrixFile.write("\n" + "b" + "," + str(i) + "," + str(j) + "," + str(mat[i][j]))


matrixFile.close()