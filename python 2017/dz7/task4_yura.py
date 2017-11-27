class Double:
    c = 0

    def __init__(self, a):
        self.c = a * 2

    def what(self):
        return self.c * 2

    def swap(self, a):
        tmp = self.c
        self.c = a.c * 2
        a.c = tmp * 2
        return

class Normal:
    c = 0

    def __init__(self, a):
        self.c = a

    def what(self):
        return self.c

    def swap(self, a):
        tmp = self.c
        self.c = a.c
        a.c = tmp
        return