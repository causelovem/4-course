# DummyClass
class Delay(object):
    slot = 0

    def __init__(self, arg):
        self.slot = arg

    def delay(self, new):
        tmp = self.slot
        self.slot = new
        return tmp
