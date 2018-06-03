class Monomial:
    def __init__(self, a, i):
        self.a = a
        self.i = i

    def __repr__(self):
        a = self.a
        i = self.i

        if a == 0:
            return ''
        elif a == 1 and i != 0:
            a = ''

        if i == 0:
            return '{0}'.format(a)
        elif i == 1:
            return '{0}x'.format(a)
        else:
            return '{0}x^{1}'.format(a, i)

    def __str__(self):
        return self.__repr__()

    @property
    def degree(self):
        return self.i
    

    def derivative(self):
        return Monomial(self.a * self.i, self.i - 1)

class Polynomial:
    def __init__(self, degree=0, coefficients=list()):
        if len(coefficients) == 0:
            coefficients = [0] * (degree + 1)

        self._coefficients = coefficients

    def __repr__(self):
        return '+'.join(map(str, self.monomials))

    def __str__(self):
        return self.__repr__()

    @property
    def degree(self):
        return len(self._coefficients) - 1
    
    @property
    def monomials(self):
        m = []

        for a, i in zip(reversed(self._coefficients), range(self.degree , -1, -1)):
            m.append(Monomial(a, i))

        return m

    def __add__(self, other):
        if isinstance(other, Monomial):
            degree = max(self.degree, other.degree)
            coefficients = self._coefficients
            coefficients[other.degree] += other.a
            return Polynomial(degree=degree, coefficients=coefficients)

    def derivative(self):
        polynomial = Polynomial(degree=self.degree-1)

        for monomial in self.monomials:
            polynomial += monomial.derivative()

        return polynomial

p = Polynomial(coefficients=[1,2,3])

print(p)
print(p.derivative())
print(Monomial(3,2))
