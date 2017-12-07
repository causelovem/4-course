# GuessABC


class A():
    val = 0

    def __init__(self, val):
        self.val = val

    def __str__(self):
        return "/{}/".format(self.val)

    def __add__(self, obj):
        return type(self)(self.val + obj.val)

    def __radd__(self, obj):
        return type(self)(self.val + obj.val)


class B():
    val = 0

    def __init__(self, val):
        self.val = val

    def __str__(self):
        return "|{}|".format(self.val)

    def __mul__(self, obj):
        return type(self)(self.val * obj.val)

    def __rmul__(self, obj):
        return type(self)(self.val * obj.val)


class C(B, A):
    val = 0

    def __init__(self, val):
        self.val = val
