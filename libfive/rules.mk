PREFIX=riscv64-unknown-elf

RV_ROOT ?= /opt/riscv

CC=$(PREFIX)-gcc
AS=$(PREFIX)-as
LD=$(PREFIX)-ld
OBJCOPY=$(PREFIX)-objcopy
OBJDUMP=$(PREFIX)-objdump

INCLUDE=-I$(LIBFIVE_ROOT)/..
ARCH=rv32im

O ?= 2

CFLAGS=-O$(O) $(INCLUDE) -g -Wall -Wno-unused-function -nostdlib -nostartfiles -ffreestanding -march=$(ARCH) -mabi=ilp32 -std=gnu99 -mcmodel=medany
ASFLAGS=-march=$(ARCH) -mabi=ilp32
LDFLAGS=-T $(LIBFIVE_ROOT)/memmap.ld -L$(RV_ROOT)/$(PREFIX)/lib/$(ARCH)/ilp32 -L$(RV_ROOT)/lib/gcc/$(PREFIX)/11.1.0/$(ARCH)/ilp32 -melf32lriscv
LDLIBS=-lgcc

LIBCSRC=$(wildcard $(LIBFIVE_ROOT)/*.c) $(wildcard $(LIBFIVE_ROOT)/libc/*.c)
LIBSSRC=$(wildcard $(LIBFIVE_ROOT)/*.s)
LIBOBJ=$(LIBCSRC:.c=.o) $(LIBSSRC:.s=.o)

SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)

all: $(PROG).hex

install: $(PROG).hex
	@test -n "$(RISCV_MOUNT)" || (echo "Set the RISCV_MOUNT environment variable to the path at which your HiFive1 RevB board is mounted. This will probably be under /media or /Volumes." && false)
	mv $< $(RISCV_MOUNT)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -c -o $@

$(PROG).elf: $(OBJ) $(LIBOBJ)
	$(LD) $(LDFLAGS) $(LIBOBJ) $(OBJ) $(LDLIBS) -o $@

%.bin: %.elf
	$(OBJCOPY) $< -O binary $@

%.hex: %.elf
	$(OBJCOPY) $< -O ihex $@

%.list: %.elf
	$(OBJDUMP) -D $< > $@

clean:
	rm -f *.o *.elf *.hex *.bin *.list

.PHONY: all install clean
