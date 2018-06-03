from typing import Tuple

def parse_color(s: str) -> Tuple[int, int, int]:
    """
    Parse color in hexadecimal string into a tuple of RGB values.
    """
    if s.startswith('#'):
        s = s[1:]

    if len(s) == 3:
        r, g, b = tuple(s)
        r *= 2
        g *= 2
        b *= 2
    elif len(s) == 6:
        r = s[0:2]
        g = s[2:4]
        b = s[4:6]
    else:
        raise ValueError('Invalid color length')

    return tuple(map(lambda x: int(x, 16), [r, g, b]))

print(parse_color('#DEF'))
