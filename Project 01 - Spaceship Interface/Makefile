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
INCL =
OBJ = $(TARGET).o
BIN = $(TARGET).bin
HEX = $(TARGET).hex

STYLE = GNU
CFFLAGS = -style=$(STYLE)

all: clean format $(HEX)

$(HEX): $(OBJS)
	$(CC) $(CFLAGS) $(SRC) -o $(OBJ) 
	$(CC) -o $(BIN) $(OBJ) $(LDFLAGS)
	$(OBJCOPY) -O ihex -R .eeprom $(BIN) $(HEX)

flash: $(HEX)
	sudo $(AVRDUDE) -F -V -c arduino -p $(MCU) -P $(PORT) -b $(BAUD_RATE) -U flash:w:$(HEX)

clean:
	rm -f $(OBJ) $(BIN) $(HEX)

format:
	$(FMT) $(CFFLAGS) -i $(SRC) $(INCL)
