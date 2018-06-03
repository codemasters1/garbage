import json
from game.units import Unit, DefenseInfo
from game.resources import Resources

__all__ = []

kinds = []
units = []

def load_units_file():
    with open('data\\units.json', 'r') as f:
        return json.load(f)

def get_all_units():
    for obj in load_units_file():
        keys = dict()
        defense_general = 0
        defense_against = None

        for k, v in obj.items():
            if k == 'costs':
                v = Resources(**v)
            elif k == 'defense':
                defense_general = v
                continue
            elif k == 'defense_against':
                defense_against = v
                continue

            keys[k] = v

        keys['defense'] = DefenseInfo(defense_general, defense_against)

        yield Unit(**keys)

def main():
    global units

    for unit in get_all_units():
        units.append(unit)

    print(units)

if __name__ == '__main__':
    main()
