from collections import OrderedDict

class Message(object):
	def __repr__(self):
		lista = []

		for field_name in message_fields.keys():
			lista.append('{0}={1}'.format(field_name, getattr(self, field_name)))


		return ', '.join(lista)

class MessageType(object):
	def __init__(self, message_type):
		self.type = message_type

	def __repr__(self):
		return self.type

class TransmissionType(object):
	def __init__(self, transmission_type):
		self.type = transmission_type

	def __repr__(self):
		return self.type

message_fields = OrderedDict([
	('message_type', MessageType),
	('transmission_type', TransmissionType),
	('session_id', str),
	('aircraft_id', str),
	('hex_ident', str),
	('flight_id', str),
	('generated_date', str),
	('generated_time', str),
	('logged_date', str),
	('logged_time', str),
	('callsign', str),
	('altitude', int),
	('ground_speed', float),
	('track', float),
	('lat', float),
	('lon', float),
	('vertical_rate', int),
	('squawk', str),
	('alert', bool),
	('emergency', bool),
	('spi', bool),
	('is_on_ground', bool)
])


def parse_message(msg):
	splitted = msg.split(',')

	message = Message()

	for field_name, field_type in message_fields.items():
		index = list(message_fields.keys()).index(field_name)
		print(index)

		if len(splitted) < index + 1:
			break

		value = splitted[index]


		if field_type in (int, float):
			if len(value) == 0:
				value = '0'

		value = field_type(value)
		setattr(message, field_name, value)

	return message