# SimpleVector


class Vector:
    v0 = 0
    v1 = 0
    v2 = 0

    def __init__(self, *arg):
        self.v0 = arg[0]
        self.v1 = arg[1]
        self.v2 = arg[2]

    def __add__(self, vec1):
        tmp = Vector(0, 0, 0)
        tmp.v0 = self.v0 + vec1.v0
        tmp.v1 = self.v1 + vec1.v1
        tmp.v2 = self.v2 + vec1.v2
        return tmp

    def __sub__(self, vec1):
        tmp = Vector(0, 0, 0)
        tmp.v0 = self.v0 - vec1.v0
        tmp.v1 = self.v1 - vec1.v1
        tmp.v2 = self.v2 - vec1.v2
        return tmp

    def __mul__(self, num):
        tmp = Vector(0, 0, 0)
        tmp.v0 = self.v0 * num
        tmp.v1 = self.v1 * num
        tmp.v2 = self.v2 * num
        return tmp

    def __rmul__(self, num):
        tmp = Vector(0, 0, 0)
        tmp.v0 = self.v0 * num
        tmp.v1 = self.v1 * num
        tmp.v2 = self.v2 * num
        return tmp

    def __truediv__(self, num):
        tmp = Vector(0, 0, 0)
        tmp.v0 = self.v0 / num
        tmp.v1 = self.v1 / num
        tmp.v2 = self.v2 / num
        return tmp

    def __matmul__(self, vec1):
        s = 0
        s += self.v0 * vec1.v0
        s += self.v1 * vec1.v1
        s += self.v2 * vec1.v2
        return s

    def __str__(self):
        s = ""
        s += "{:.2f}".format(self.v0)
        s += ":{:.2f}".format(self.v1)
        s += ":{:.2f}".format(self.v2)
        return s
