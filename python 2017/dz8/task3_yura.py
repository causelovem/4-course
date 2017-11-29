class Unary():

    N = 0
    current = 0

    def __init__(self, arg):
        self.N = len(arg)

    def __str__(self):
        return "".join(["|" for s in range(self.N)])

    def __len__(self):
        return self.N

    def __ior__(self, other):
        self.N += other.N
        return self

    def __invert__(self):
        self.N //= 2
        return self

    def __pos__(self):
        self.N += 1
        return self

    def __iter__(self):
        return self

    def __next__(self):
        if self.current >= self.N:
            self.current = 0
            raise StopIteration
        else:
            self.current += 1
            return Unary("|")
