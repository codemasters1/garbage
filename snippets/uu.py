import math
import random
import re

def extract_parts(text):
	parts = re.split(r'(-?\d+)', text)

	# Remove empty parts
	parts = filter(None, parts)

	return parts


def is_number(text):
	return (text[1:] if text.startswith('-') else text).isdigit()

def prepare_template(text):
	parts = []
	numbers = []

	for part in extract_parts(text):
		if is_number(part):
			numbers.append(int(part))
			parts.append('{}')
		else:
			parts.append(part)

	return ''.join(parts), numbers


text = 'plujena-34rozneo5oby'

template, numbers = prepare_template(text)

print(text, '=>', template.format(*reversed(numbers), numbers))
