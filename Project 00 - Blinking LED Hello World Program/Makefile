CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude
FMT = clang-format
MCU = atmega328p
F_CPU = 16000000UL
BAUD_RATE = 115200
PORT = /dev/ttyACM0

CFLAGS = -Os -DF_CPU=$(F_CPU) -mmcu=$(MCU) -c
LDFLAGS =

TARGET = HelloWorld
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
	$(CC) $(CFLAGS) $(SRC) #-o $(OBJS) 
	$(CC) -o $(BIN) $(OBJ)
	$(OBJCOPY) -O ihex -R .eeprom $(BIN) $(HEX)

flash: $(HEX)
	sudo $(AVRDUDE) -F -V -c arduino -p $(MCU) -P $(PORT) -b $(BAUD_RATE) -U flash:w:$(HEX)

clean:
	rm -f $(OBJ) $(BIN) $(HEX)

format:
	$(FMT) $(CFFLAGS) -i $(SRC) $(INCL)
