# Build an ELF by
# .asm -(CPP)-> .asmx -(CPP)-> .s -(AS)-> .o -> (LD) -> .elf
#
# Due to the way the TI include file msp430g2553.h is written, we need
# to preprocess the file twice. We need to use -traditional-cpp,
# otherwise cpp defines __STDC__ which will lead to inclusion of some
# C code that msp430-as doesn't accept. However, if we use
# -traditional-cpp, cpp won't remove the C++ style comments, so
# msp430-as doesn't accept this either.
#
# (The alternative would have been to use one pass with -U __STDC__, but that
# results in a warning about undefining a standard define.)

MCU=msp430g2553
CPU=430
MPY=none

AS=msp430-as
ASFLAGS=-mmcu=$(MCU) -mcpu=$(CPU) -mmpy=$(MPY)
CPP=cpp
CPPFLAGS=-I /usr/msp430/include -x assembler-with-cpp
LD=msp430-ld
LDFLAGS=-L /usr/msp430/lib/ldscripts/$(MCU)
SIZE=msp430-size

PROG_NAME=cycle_led
EXECUTABLE=$(PROG_NAME).elf
OBJS=$(PROG_NAME).o

.PHONY: all clean

all: $(EXECUTABLE)

clean:
	rm -fr $(EXECUTABLE) $(OBJS)

$(EXECUTABLE): $(OBJS)
	$(LD) $(LDFLAGS) -o $(EXECUTABLE) $<
	@echo
	@$(SIZE) $(EXECUTABLE)
	@echo

%.asmx: %.asm
	$(CPP) $(CPPFLAGS) -traditional-cpp $< > $@ 

%.s: %.asmx
	$(CPP) $(CPPFLAGS) $< > $@ 

%.o: %.s
	$(AS) $(ASFLAGS) -o $@ $<
