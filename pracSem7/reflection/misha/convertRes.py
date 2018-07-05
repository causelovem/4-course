import sys

outDir = sys.argv[1]

for i in range(2, len(sys.argv)):
    resIn = open(sys.argv[i], 'r')
    file = resIn.readlines()

    outName = outDir + sys.argv[i][3:]
    resOut = open(outName, 'w')

    for j in range(-5, 0):
        resOut.write(file[j])

    resIn.close()
    resOut.close()
