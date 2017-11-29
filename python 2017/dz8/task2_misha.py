# StrangeDots


class Dots:
    st = 0
    ed = 0

    def __init__(self, *arg):
        self.st = arg[0]
        self.ed = arg[1]

    def __getitem__(self, item):
        res = []
        cnt = self.ed - self.st
        if isinstance(item, slice):
            if item.step is None:
                step = cnt / (item.stop - 1)

                res = self.st + step * item.start
            else:
                st = item.start
                ed = item.stop

                if item.start is None:
                    st = 0
                if item.stop is None:
                    ed = item.step
                step = cnt / (item.step - 1)

                i = self.st + st * step
                while i < ed * step:
                    if abs(i) < 0.00000000001:
                        i = 0.0
                    res.append(float(i))
                    i += step
        else:
            step = cnt / (item - 1)

            i = self.st
            while i <= self.ed:
                if abs(i) < 0.00000000001:
                    i = 0.0
                res.append(float(i))
                i += step
        return res


a = Dots(-1,1)
print(*a[7])
print(a[0:7])
print(a[2:7])
print(a[4:7])
print(a[7:7])
print(a[-7:7])
print(*a[1:3:7])
print(*a[:3:7])
print(*a[2::7])
print(*a[::7])
print(*a[-2:8:7])