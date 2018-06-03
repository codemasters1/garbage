from game.resources import Resources

__all__ = ['Unit', 'DefenseInfo']

class DefenseInfo(object):
    def __init__(self, general=0, against=None):
        self._general = general
        self._against = against

    def __int__(self):
        return self._general

    def __repr__(self):
        return '<Defense general={0} against={1}>'.format(self._general, self._against)

class Unit(object):
    def __init__(self, name, kind, population=1, speed=1, carry=0, costs=None, attack=0, defense=None):
        self._name = name
        self._kind = str(kind)
        self._population = int(population)
        self._speed = int(speed)
        self._carry = int(carry)
        self._costs = costs or Resources(0, 0, 0)
        self._attack = int(attack)
        self._defense = defense

    @property
    def kind(self):
        return self._kind

    @property
    def population(self):
        return self._population

    @property
    def speed(self):
        return self._speed

    @property
    def carry(self):
        return self._carry

    @property
    def costs(self):
        return self._costs

    @property
    def attack(self):
        return self._attack

    @property
    def defense(self):
        return self._defense

    def __repr__(self):
        return '<Unit kind={0} population={1} speed={2} carry={3} costs={4} attack={5} defense={6}>' \
            .format(self._kind, self._population, self._speed, self._carry, self._costs, self._attack, self._defense)


