# SemDescriptor


class Desc():
    name = None

    def __get__(self, obj, cls):
        if self.name is None:
            self.name = obj.name
            return None

        return "<{}>".format(self.name)

    def __delete__(self, obj):
        if obj.name == self.name:
            self.name = None


class Sem():
    lock = Desc()
    name = ""

    def __init__(self, name):
        self.name = name
