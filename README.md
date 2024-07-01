# Arduino Projects Book in C

All the projects from the [Arduino Projects Book](https://store-usa.arduino.cc/products/arduino-starter-kit-multi-language), written in C.

## Building and running
Each project is separated into its own folder, with each folder having its own makefile. To run a particular project, clone the repo onto your machine, cd into the corresponding directory, run `make` to build the executable, and then flash the program onto your Arduino using the `make flash` command.

**NOTE**: Please double check which port your Arduino is located on using `ls /dev/ | grep ACM` then set the `PORT` variable in that project's `Makefile` to the output of that command.

In order to build and run these projects, you will need the following libraries installed on your machine: `avr-binutils`, `avr-gcc`, `avrdude`, and `avr-libc`.

## Background
As someone with an interest in computer engineering, low-level programming, and embedded systems, I'm using this project as an opportunity to work closer to the bare-metal of the Arduino Uno. This project was started in the hopes that it could be a stepping stone for me towards working with more professional/industry-grade boards (like the STM32 boards) in the future.

## Specs
* **Microcontroller**: ATmega328P @ 16 MHz
* **Memory**: 2KB SRAM, 32KB FLASH, 1KB EEPROM
* **USB-Serial Processor**: ATmega16U2 @ 16 MHz
* Peripherals
  * 2x 8-bit Timer/Counter with a dedicated period register and compare channels
  * 1x 16-bit Timer/Counter with a dedicated period register, input capture and compare channels
  * 1x USART with fractional baud rate generator and start-of-frame detection
  * 1x controller/peripheral Serial Peripheral Interface (SPI)
  * 1x Dual mode controller/peripheral I2C
  * 1x Analog Comparator (AC) with a scalable reference input
  * Watchdog Timer with separate on-chip oscillator
  * Six PWM channels
  * Interrupt and wake-up on pin change

For more details, [read here](https://docs.arduino.cc/hardware/uno-rev3/).
