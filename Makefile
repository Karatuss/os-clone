ARCH = armv7-a
MCPU = cortex-a8

TARGET = rvpb

CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-gcc
OC = arm-none-eabi-objcopy

LINKER_SCRIPT = ./navilos.ld
MAP_FILE = build/navilos.map

ASM_SRCS = $(wildcard boot/*.S)
ASM_OBJS = $(patsubst boot/%.S, build/%.os, $(ASM_SRCS))

VPATH = boot			\
		hal/$(TARGET)	\
		lib				\
		kernel	

C_SRCS  = $(notdir $(wildcard boot/*.c))
C_SRCS += $(notdir $(wildcard hal/$(TARGET)/*.c))
C_SRCS += $(notdir $(wildcard lib/*.c))
C_SRCS += $(notdir $(wildcard kernel/*.c))
C_OBJS = $(patsubst %.c, build/%.o, $(C_SRCS))

INC_DIRS = -I include		\
		   -I hal			\
		   -I hal/$(TARGET) \
		   -I lib			\
		   -I kernel


CFLAGS = -c -g -std=c11
LDFLAGS = -nostartfiles -nostdlib -nodefaultlibs -static -lgcc

navilos = build/navilos.axf
navilos_bin = build/navilos.bin

.PHONY = all clean run debug gdb kill

all: $(navilos)

clean:
	@rm -rf build
	@rm -rf boot/Entry.bin boot/Entry.o

run: $(navilos)
	qemu-system-arm -M realview-pb-a8 \
		-kernel $(navilos) \
		-nographic

debug: $(navilos)
	qemu-system-arm -M realview-pb-a8 \
		-kernel $(navilos) \
		-nographic \
		-S -gdb tcp::1234,ipv4

gdb:
	arm-none-eabi-gdb --command=./gdbscript

kill:
	kill -9 `ps aux | grep qemu | awk 'NR==1{print $$2}'`

$(navilos): $(ASM_OBJS) $(C_OBJS) $(LINKER_SCRIPT)
	$(LD) -n -T $(LINKER_SCRIPT) -o $(navilos) $(ASM_OBJS) $(C_OBJS) \
		  -Wl,-Map=$(MAP_FILE) $(LDFLAGS)
	$(OC) -O binary $(navilos) $(navilos_bin)

build/%.os: %.S
	mkdir -p $(shell dirname $@)
	$(CC) -mcpu=$(MCPU) -marm \
		  $(INC_DIRS) \
		  $(CFLAGS) -o $@ $<

build/%.o: %.c
	mkdir -p $(shell dirname $@)
	$(CC) -mcpu=$(MCPU) -marm \
		  $(INC_DIRS) \
		  $(CFLAGS) -o $@ $<
