# TrianglesCmp

import math

class Triangle:
    x = 0
    y = 0
    z = 0
    emp = 0

    def __init__(self, *arg):
        self.x = arg[0]
        self.y = arg[1]
        self.z = arg[2]

        if ((self.x <= 0) or (self.y <= 0) or (self.z <= 0) or (self.x + self.y < self.z) or (self.x + self.z < self.y) or (self.z + self.y < self.x)):
            self.emp = 1

    def __abs__(self):
        p = (self.x + self.y + self.z) / 2

        if self.emp == 1:
            return 0
        return math.sqrt(p * (p - self.x) * (p - self.y) * (p - self.z))

    def __eq__(self, obj):
        if ((self.x == obj.x) or (self.x == obj.y) or (self.x == obj.z)):
            if ((self.y == obj.x) or (self.y == obj.y) or (self.y == obj.z)):
                if ((self.z == obj.x) or (self.z == obj.y) or (self.z == obj.z)):
                    return True
        return False

    def __ne__(self, obj):
        if (self.__abs__() == obj.__abs__()):
            return False
        return True

    def __bool__(self):
        if (self.emp == 0):
            return True
        return False

    def __lt__(self, obj):
        if (self.__abs__() < obj.__abs__()):
            return True
        return False

    def __gt__(self, obj):
        if (self.__abs__() > obj.__abs__()):
            return True
        return False

    def __le__(self, obj):
        if (self.__abs__() <= obj.__abs__()):
            return True
        return False

    def __ge__(self, obj):
        if (self.__abs__() >= obj.__abs__()):
            return True
        return False

    def __str__(self):
        s = ""
        s += "{:.1f}".format(self.x)
        s += ":{:.1f}".format(self.y)
        s += ":{:.1f}".format(self.z)
        return s
