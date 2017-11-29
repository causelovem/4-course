import math

class Triangle(object):
    a = 0.0
    b = 0.0
    c = 0.0

    def __init__(self, x, y, z):
        (self.a, self.b, self.c) = (x, y, z)

    def isEmpty(self):
        if (self.a + self.b > self.c) and (self.a + self.c > self.b) and (self.c + self.b > self.a):
            return True
        else:
            return False

    def __abs__(self):
        if self.isEmpty():
            return 0
        else:
            p = (a + b + c) / 2
            return math.sqrt(p* (p - a) * (p - b) * (p - c))
    
    # def __cmp__(self, other):
    #     if (self.a == other.a) and (self.b == other.b) and (self.c == other.c):
    #         return 0
    #     if (self.a == other.a) and (self.b == other.c) and (self.c == other.b):
    #         return 0
    #     if (self.a == other.b) and (self.b == other.b) and (self.c == other.c):
    #         return 0

    #     if abs(self) < abs(other):
    #         return -1

    #     return 1

    def __str__(self):
        return "{:.2f}:{:.2f}:{:.2f}".format(float(self.a), float(self.b), float(self.c))

Tri = Triangle(3,4,5), Triangle(5,4,3), Triangle(7,1,1), Triangle(5,5,5), Triangle(7,4,4)
for a,b in zip(Tri[:-1],Tri[1:]):
    print(a if a else b)
    print("{}={:.2f} {}={:.2f}".format(a, abs(a), b, abs(b)))
    print(a == b)
    print(a >= b)
    print(a < b)