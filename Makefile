CC = gcc
AS = nasm
LD = ld
OBJCOPY = objcopy

SRC_DIR = ./kernel
INC_DIR = ./include
BUILD_DIR = ./build
HOME_DIR = /home/pororo

ASFLAGS = -f elf64
CFLAGS = -m64 -I$(INC_DIR) -ffreestanding -fno-stack-protector -mno-red-zone
LDFLAGS = -m elf_x86_64 -T $(SRC_DIR)/kernel.ld -static -nostdlib

TARGET = TOSKernel.elf
OBJS = $(BUILD_DIR)/init.o $(BUILD_DIR)/kernel.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^
	cp $@ $(HOME_DIR)/run-ovmf/hda-contents/

$(BUILD_DIR)/init.o: $(SRC_DIR)/init.S
	@mkdir -p $(BUILD_DIR)
	$(AS) $(ASFLAGS) $< -o $@

$(BUILD_DIR)/kernel.o: $(SRC_DIR)/kernel.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
