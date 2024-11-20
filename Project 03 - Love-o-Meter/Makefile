CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude
FMT = clang-format
MCU = atmega328p
F_CPU = 16000000UL
BAUD_RATE = 115200
PORT = /dev/ttyACM0

CFLAGS = -O1 -std=c99 -Wpedantic -Wextra -Werror -Wall -Wstrict-aliasing=3 -Wwrite-strings -Wvla -Wcast-align=strict -Wstrict-prototypes -Wstringop-overflow=4 -Wshadow -fanalyzer -DF_CPU=$(F_CPU) -mmcu=$(MCU) -c
LDFLAGS = -DF_CPU=$(F_CPU) -mmcu=$(MCU)

TARGET = main
SRC_DIR=src
SRC = $(SRC_DIR)/*.c
INCL = $(SRC_DIR)/*.h
OBJ = *.o
BIN = $(TARGET).bin
HEX = $(TARGET).hex

STYLE = GNU
CFFLAGS = -style=$(STYLE)

#all: clean erase format $(HEX)
all: clean format $(HEX)

$(HEX): $(OBJS)
	$(CC) $(CFLAGS) $(SRC)
	$(CC) -o $(BIN) *.o $(LDFLAGS)
	$(OBJCOPY) -O ihex -R .eeprom $(BIN) $(HEX)

flash: $(HEX)
	sudo $(AVRDUDE) -F -V -c arduino -p $(MCU) -P $(PORT) -b $(BAUD_RATE) -U flash:w:$(HEX)

clean:
	rm -f $(OBJ) $(BIN) $(HEX)

erase:
	# Clear the Arduino's flash memory
	-@sudo $(AVRDUDE) -F -V -c arduino -p $(MCU) -P $(PORT) -b $(BAUD_RATE) -e

connect:
	sudo cu -l /dev/ttyACM0 -s 9600

format:
	$(FMT) $(CFFLAGS) -i $(SRC) $(INCL)
