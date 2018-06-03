#
# Copyright (C) Patryk Jaworski <regalis@regalis.com.pl>
# 
# Makefile for Arduino projects wihout high-level library.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.
# 
 
# Makefile for Arduino UNO
 
AVRDUDE_UPLOAD_SPEED=57600
AVRDUDE_COM_PORT?=COM6
AVRDUDE_PROGRAMMER=arduino
AVRDUDE_MCU=m328p
 
MCU=atmega328p
CC_FLAGS=-std=c99 -Os -Wall -mmcu=$(MCU) -DF_CPU=$(F_CPU) -c
LD_FLAGS=-mmcu=$(MCU)
 
F_CPU=16000000UL
CC=avr-gcc
CXX=avr-g++
LD=avr-gcc
OBJ_COPY=avr-objcopy
 
OBJS=$(patsubst %.c, %.o, $(wildcard *.c))
 
main.elf: $(OBJS)
	$(LD) $(LD_FLAGS) $^ -o $@
 
main.hex: main.elf
	$(OBJ_COPY) -O ihex -R .eeprom $< $@
 
%.o: %.c
	$(CC) $(CC_FLAGS) $< -o $@
 
%.o: %.c %.h
	$(CC) $(CC_FLAGS) $< -o $@
 
%.o: %.cpp
	$(CXX) $(CC_FLAGS) $< -o $@
 
%.o: %.cpp %.h
	$(CXX) $(CC_FLAGS) $< -o $@
 
upload: main.hex
	avrdude -c $(AVRDUDE_PROGRAMMER) -p $(AVRDUDE_MCU) -b $(AVRDUDE_UPLOAD_SPEED) -P $(AVRDUDE_COM_PORT) -U flash:w:$<
 
clean:
	rm -rvf *.o main.elf main.hex
 
.PHONY: upload clean