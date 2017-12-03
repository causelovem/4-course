import math

class Triangle(object):
    a = 0.0
    b = 0.0
    c = 0.0

    def __init__(self, x, y, z):
        (self.a, self.b, self.c) = (x, y, z)

    def isEmpty(self):
        if (self.a + self.b > self.c) and (self.a + self.c > self.b) and (self.c + self.b > self.a):
            return False
        else:
            return True

    def __abs__(self):
        if self.isEmpty():
            return 0
        else:
            p = (self.a + self.b + self.c) / 2
            return math.sqrt(p*(p - self.a)*(p - self.b)*(p - self.c))
    
    def __eq__(self, other):
        if ((self.a == other.a) or (self.a == other.b) or (self.a == other.c)):
            if ((self.b == other.a) or (self.b == other.b) or (self.b == other.c)):
                if ((self.c == other.a) or (self.c == other.b) or (self.c == other.c)):
                    return True
        return False

    def __ne__(self, other):
        return not (self == other)

    def __lt__(self, other):
        if (self.__abs__() < other.__abs__()):
            return True
        return False

    def __gt__(self, other):
        if (self.__abs__() > other.__abs__()):
            return True
        return False

    def __le__(self, other):
        if (self.__abs__() <= other.__abs__()):
            return True
        return False

    def __ge__(self, other):
        if (self.__abs__() >= other.__abs__()):
            return True
        return False

    def __str__(self):
        return "{:.1f}:{:.1f}:{:.1f}".format(float(self.a), float(self.b), float(self.c))

    def __bool__(self):
        if not self.isEmpty():
            return True
        return False