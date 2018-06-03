def split_into_char_groups(string):
    group = ''

    for char in string:
        if not group or group[-1] == char:
            group += char
        else:
            yield group
            group = char

    if group:
        yield group


def compress(string):
    result = ''

    for group in split_into_char_groups(string):
        if len(group) > 2:
            result += group[0] + str(len(group))
        else:
            result += group

    return result

num_lines = int(input(''))

for i in range(0, num_lines):
    print(compress(input()))
