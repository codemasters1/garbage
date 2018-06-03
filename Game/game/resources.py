class Resources(object):
    def __init__(self, wood=0, stone=0, iron=0):
        self._wood = int(wood)
        self._stone = int(stone)
        self._iron = int(iron)

    @property
    def wood(self):
        return self._wood

    @property
    def stone(self):
        return self._stone

    @property
    def iron(self):
        return self._iron

    def __add__(self, other):
        r = dict(
            wood=self.wood+other.wood,
            stone=self.wood+other.wood,
            iron=self.iron+other.iron
        )
        return Resources(**r)

    def __repr__(self):
        return '<Resources wood={0} stone={1} iron={2}>'.format(self._wood, self._stone, self._iron)