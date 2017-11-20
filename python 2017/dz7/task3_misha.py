# SharedBrain
class Overall(object):
    over = [0]

    def click(self):
        self.over[0] += 1
        return self.over[0]
