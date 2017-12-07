# MegaStroka


class Stroka(str):
    def __neg__(self):
        return self.str[::-1]

    def __mul__(self, obj):
        if isinstance(obj, int):
            return str(self) * obj

        res = ""
        for i in self:
            for j in obj:
                res += i + j
        return Stroka(res)

    def __pow__(self, p):
        if p == 0:
            return ""

        res = self
        for i in range(p - 1):
            res = Stroka(res) * self
        return Stroka(res)
