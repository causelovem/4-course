class Vector():
    x = 0.0
    y = 0.0
    z = 0.0

    def __init__(self, a, b, c):
        (self.x, self.y, self.z) = (a, b, c)

    def __add__(self, other):
        return Vector(self.x + other.x, self.y + other.y, self.z + other.z)

    def __sub__(self, other):
        return Vector(self.x - other.x, self.y - other.y, self.z - other.z)
        
    def __mul__(self, other):
        return Vector(self.x * other, self.y * other, self.z * other)

    def __truediv__(self, other):
        return Vector(self.x / other, self.y / other, self.z / other)

    def __rmul__(self, other):
        return self.__mul__(other)

    def __matmul__(self, other):
        return self.x * other.x + self.y * other.y + self.z * other.z 

    def __str__(self):
        return "{:.2f}:{:.2f}:{:.2f}".format(float(self.x), float(self.y), float(self.z))