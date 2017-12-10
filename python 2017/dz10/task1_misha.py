# BoldCalc


def parser(com):
    if com[0] == '#':
        return ''

    left = 0
    right = 0

    if com.find('=') != -1:
        left = com[:com.find('=')]
        if not left.isidentifier():
            raise Exception("invalid identifier '{}'".format(left))

        right = com[com.find('=') + 1:]
        if right.find('=') != -1:
            raise Exception("invalid assignment '{}'".format(com))

        var[left] = eval(right, var)
        return ''

    return eval(com, var)


var = {}

inp = input()

while (inp != ''):
    try:
        out = parser(inp)
        if (((isinstance(out, str)) and (len(out) != 0)) or (not isinstance(out, str))):
            print(out)
    except Exception as excep:
        print(excep)

    inp = input()
