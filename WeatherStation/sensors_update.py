from __future__ import print_function

from serial import Serial
from sqlalchemy import create_engine, Table, MetaData, Column, Integer, String, DateTime, Float, select
from datetime import datetime

engine = create_engine('mysql://root:makakuwa@localhost/weather_station', echo=True)
meta = MetaData(engine)
connection = engine.connect()


temp_sensors = Table('temp_sensors', meta, 
	Column('id', Integer, primary_key=True),
	Column('address', String(13), unique=True),
	Column('name', String(100), unique=True),
)

temps = Table('temps', meta, 
	Column('id', Integer, primary_key=True),
	Column('sensor_id', Integer, nullable=False),
	Column('time', DateTime, nullable=False),
	Column('temperature', Float, nullable=False)
)

meta.create_all()

class TemperatureManager(object):
	def __init__(self, port):
		self._serial = Serial(port)

	def read_temps(self):
		temps = {}

		self._serial.write(b'read_all')

		while True:
			line = self._serial.readline().strip()

			if not line:
				break

			address, temp = line.split()

			temps[address] = float(temp)

		return temps

def main():
	man = TemperatureManager('/dev/ttyUSB0')

	for addr, temp in man.read_temps().items():
		print(addr, temp)

		q = select([temp_sensors.c.id]).where(temp_sensors.c.address == addr)
		sensor_id = connection.execute(q).scalar()

		if sensor_id is None:
			result = connection.execute(temp_sensors.insert(), address=addr, name=addr)
			sensor_id = result.lastrowid

		connection.execute(temps.insert(), sensor_id=sensor_id, temperature=temp, time=datetime.now())

if __name__ == '__main__':
	main()