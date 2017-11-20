# NormalDouble
class Normal:
    p = 0

    def __init__(self, arg):
        self.p = arg

    def what(self):
        return self.p

    def swap(self, arg):
        tmp = self.p
        self.p = arg.p
        arg.p = tmp
        return


class Double:
    p = 0

    def __init__(self, arg):
        self.p = arg * 2

    def what(self):
        return self.p * 2

    def swap(self, arg):
        tmp = self.p
        self.p = arg.p * 2
        arg.p = tmp * 2
        return
