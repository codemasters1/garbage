from operator import itemgetter
from collections import namedtuple
from random import randint

MIN_AGE = 21
MAX_AGE = 50
NAME_LENGTH = 7
ALLOWED_NAME_CHARS = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz'
NUMBER_OF_DEVELOPERS = 10

Developer = namedtuple('Developer', ['name', 'age'])

def sort_by_age(developers):
    return sorted(developers, key=itemgetter(1))

def random_age():
	return randint(MIN_AGE, MAX_AGE)

def random_string(allowed_chars, length):
    return ''.join((allowed_chars[randint(0, len(allowed_chars) - 1)] for _ in range(length)))

def random_name():
    return random_string(ALLOWED_NAME_CHARS, NAME_LENGTH)

def create_developers(amount):
    return [Developer(random_name(), random_age()) for _ in range(amount)]

developers = create_developers(NUMBER_OF_DEVELOPERS)
print(sort_by_age(developers))
