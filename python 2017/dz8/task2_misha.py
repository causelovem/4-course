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

                for i in range(st, ed):
                    res.append(float(self.st + i * step))
        else:
            step = cnt / (item - 1)

            for i in range(0, item):
                res.append(float(self.st + i * step))
        return res
