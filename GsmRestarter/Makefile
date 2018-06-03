F_CPU=16000000UL

AVRDUDE_PROGRAMMER=usbasp
AVRDUDE_MCU=m328p
 
MCU=atmega328p
CC_FLAGS=-std=c99 -Os -Wall -Werror -mmcu=$(MCU) -DF_CPU=$(F_CPU) -c
LD_FLAGS=-mmcu=$(MCU)

SRC=src
BIN=bin

CC=avr-gcc
LD=avr-gcc
OBJ_COPY=avr-objcopy
 
OBJS=$(patsubst %.c, %.o, $(wildcard $(SRC)/*.c))


main.elf: $(OBJS)
	$(LD) $(LD_FLAGS) $^ -o $(BIN)/$@
 
main.hex: main.elf
	$(OBJ_COPY) -O ihex -R .eeprom $(BIN)/$< $(BIN)/$@
 
%.o: %.c
	mkdir -p $(BIN) $< $@
	$(CC) $(CC_FLAGS) $< -o $(BIN)/$@
 
%.o: %.c %.h
	mkdir -p $(BIN)
	$(CC) $(CC_FLAGS) $< 
 
upload: main.hex
	avrdude -c $(AVRDUDE_PROGRAMMER) -p $(AVRDUDE_MCU) -U flash:w:$(BIN)/$<
 
clean:
	rm -rf $(BIN) main.elf main.hex

test:
	echo $(wildcard $(SRC)/*.c)
 
.PHONY: upload clean