CC      := ppu-gcc
OBJCOPY := ppu-objcopy

TARGET = payload

CFLAGS = -std=c99 -m64 -fno-builtin -fno-exceptions -fdata-sections -ffunction-sections -Os -Wno-strict-aliasing -Wno-multichar -Wall -I.
ifeq ($(BUILD_TYPE),debug)
	CFLAGS += -DDEBUG
endif
CFLAGS += -D__STDC_FORMAT_MACROS
ASFLAGS = -m64 -Os -Wall -I.
ASFLAGS += -D__ASSEMBLY__
ifeq ($(BUILD_TYPE),debug)
	ASFLAGS += -DDEBUG
endif
LDFLAGS = -Tldscript.ld -nostartfiles -nostdlib -nodefaultlibs -Wl,--gc-sections -Wl,-Map=$(TARGET)_$@.map

PAYLOADS := 421C 421D 430C 430D 431C 440C 441C 441D 446C 446D 450C 450D 453C 453D 455C 455D 460C \
			465C 465D 470C 470D 475C 475D 476C 476D 478C 478D 480C 480D 481C 481D 482C 482D

define do_payload
	$(CC) $(ASFLAGS) -DFIRMWARE_$@ -c start.S -o start.S.o
	$(CC) $(ASFLAGS) -DFIRMWARE_$@ -c kernel.S -o kernel.S.o
	$(CC) $(ASFLAGS) -DFIRMWARE_$@ -c hypervisor.S -o hypervisor.S.o
	$(CC) $(ASFLAGS) -DFIRMWARE_$@ -c syscalls.S -o syscalls.S.o
	$(CC) $(ASFLAGS) -DFIRMWARE_$@ -c util.S -o util.S.o
	$(CC) $(CFLAGS) -DFIRMWARE_$@  -c main.c -o main.c.o
	$(CC) $(CFLAGS) -DFIRMWARE_$@  -c device.c -o device.c.o
	$(CC) $(CFLAGS) -DFIRMWARE_$@  -c network.c -o network.c.o
	$(CC) $(CFLAGS) -DFIRMWARE_$@  -c mm.c -o mm.c.o
	$(CC) $(CFLAGS) -DFIRMWARE_$@  -c kernel.c -o kernel.c.o
	$(CC) $(CFLAGS) -DFIRMWARE_$@  -c hypervisor.c -o hypervisor.c.o
	$(CC) $(CFLAGS) -DFIRMWARE_$@  -c spu.c -o spu.c.o
	$(CC) $(CFLAGS) -DFIRMWARE_$@  -c debug.c -o debug.c.o
	$(CC) $(CFLAGS) -DFIRMWARE_$@  -c util.c -o util.c.o
	$(CC) $(CFLAGS) -DFIRMWARE_$@  -c dumper.c -o dumper.c.o
	$(CC) $(LDFLAGS) -L. -o $(TARGET)_$@.elf start.S.o kernel.S.o hypervisor.S.o syscalls.S.o util.S.o main.c.o device.c.o network.c.o mm.c.o kernel.c.o hypervisor.c.o spu.c.o debug.c.o util.c.o dumper.c.o
	$(OBJCOPY) -O binary $(TARGET)_$@.elf $(TARGET)_$@.bin
	mv -f $(TARGET)_$@.bin ../../data/$(TARGET)_$@.bin
	rm -f *.o *.elf *.bin *.map
endef

			
all: $(PAYLOADS)

$(PAYLOADS):
	$(do_payload)
	@echo ----------------------------------------------------------------

clean:
	rm -f *.o *.elf *.bin *.map
