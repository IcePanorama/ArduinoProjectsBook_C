CC = avr-gcc
# `utils/delay.h` throws a warning when compiling w/o optimizations
CFLAGS = -O1 -std=c99 -Wpedantic -Wextra -Werror -Wall -Wstrict-aliasing=3
CFLAGS += -Wwrite-strings -Wvla -Wcast-align=strict -Wstrict-prototypes
CFLAGS += -Wstringop-overflow=4 -Wshadow -fanalyzer -DF_CPU=$(F_CPU)
CFLAGS += -mmcu=$(MCU)
LDFLAGS = -DF_CPU=$(F_CPU) -mmcu=$(MCU)

OBJCOPY = avr-objcopy
AVR_FLASH = avrdude
FMT = clang-format

# Settings for flashing
BAUD_RATE = 115200
PORT = /dev/ttyACM0

MCU = atmega328p
F_CPU = 16000000UL

TARGET = main
SRC_DIR=src
INCL_DIR=include
SRC = $(SRC_DIR)/adc.c \
      $(SRC_DIR)/main.c \
      $(SRC_DIR)/love_o_meter.c \
      $(SRC_DIR)/uart_hal.c
INCL = $(INCL_DIR)/adc.h \
       $(INCL_DIR)/uart_hal.h \
       $(INCL_DIR)/love_o_meter.h
BIN = $(TARGET).bin
HEX = $(TARGET).hex

STYLE = GNU
FMT_FLAGS = -style=$(STYLE)

all: clean format $(HEX)

$(HEX): $(OBJS)
	$(CC) -c $(CFLAGS) $(SRC) -I$(INCL_DIR)/
	$(CC) -o $(BIN) *.o $(LDFLAGS)
	$(OBJCOPY) -O ihex -R .eeprom $(BIN) $(HEX)

flash: $(HEX)
	sudo $(AVR_FLASH) -F -V -c arduino -p $(MCU) -P $(PORT) -b $(BAUD_RATE) -U flash:w:$(HEX)

clean:
	rm -f $(OBJ) $(BIN) $(HEX)

# Clear the Arduino's flash memory
erase:
	-@sudo $(AVR_FLASH) -F -V -c arduino -p $(MCU) -P $(PORT) -b $(BAUD_RATE) -e

connect:
	sudo cu -l $(PORT) -s 9600

format:
	$(FMT) $(FMT_FLAGS) -i $(SRC) $(INCL)
