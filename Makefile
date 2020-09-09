PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
OBJCOPY = $(PREFIX)objcopy
LD = $(PREFIX)ld

SRCS = $(wildcard src/*.c)

CFLAGS  = -mlittle-endian
CFLAGS += -mthumb
CFLAGS += -mcpu=cortex-m4
CFLAGS += -mfloat-abi=hard
CFLAGS += -mfpu=fpv4-sp-d16
CFLAGS += -ggdb
CFLAGS += -Wall -O2

CFLAGS += -I include/CMSIS
CFLAGS += -I include/

.PHONY: all clean

all: main.elf
	$(OBJCOPY) -O binary -S $< main.bin

main.elf: build/main.o build/Startup.o build/i2c.o build/utils.o
	$(LD) -nostartfiles -T Flash.ld -Map main.map -o $@ $^

build/%.o: src/%.c
	if [ ! -d "./build" ]; then mkdir build; fi
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf *.o *.elf *.bin build
