# UnaryNumber


class Unary:
    num = 0

    def __init__(self, n):
        self.num = len(n)

    def __str__(self):
        return "|" * self.num

    def __len__(self):
        return self.num

    def __ior__(self, n):
        self.num += n.num
        return self

    def __invert__(self):
        self.num //= 2
        return self

    def __pos__(self):
        self.num += 1
        return self

    def __iter__(self):
        return (Unary("|") for i in range(self.num))

