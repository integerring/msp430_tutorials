MCU=msp430g2452
# MCU=msp430g2553

CC=msp430-gcc
CFLAGS=-Os -Wall -mmcu=$(MCU) -mdisable-watchdog
SIZE=msp430-size

PROG_NAME=check_xtal
EXECUTABLE=$(PROG_NAME).elf
OBJS=$(PROG_NAME).o

.PHONY: all clean

all: $(EXECUTABLE)

clean:
	rm -fr $(EXECUTABLE) $(OBJS)

$(EXECUTABLE): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(OBJS)
	@echo
	@$(SIZE) $(EXECUTABLE)
	@echo

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $<
