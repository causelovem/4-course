class Dots:
    a = 0.0
    b = 0.0

    def __init__(self, x, y):
        (self.a, self.b) = (x, y)

    def __getitem__(self, item):
        if isinstance(item, slice):
            if item.step is None:
                return self.a + (self.b - self.a) * item.start / (item.stop - 1)
            else:
                mas = []
                cnt = item.start
                end = item.stop
                if item.start is None:
                    cnt = 0
                if item.stop is None:
                    end = item.step

                while cnt < end:
                    mas.append(self[cnt:item.step])
                    cnt += 1
                return mas
        else:
            mas = []
            step = (self.b - self.a) / (item - 1)
            i = self.a
            while i <= self.b:
                mas.append(i)
                i += step
            return mas