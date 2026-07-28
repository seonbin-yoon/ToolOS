CC = gcc
ASM = nasm
LD = ld
# OBJCOPY = objcopy

KERNEL = ./kernel
INCLUDE = ./include
TMP = ./build

ASMFLAGS = -f elf64
CFLAGS = -g -m64 -O2 -Wall -ffreestanding -nostdinc -fno-stack-protector -mno-red-zone -I$(INCLUDE) # -finline-functions
LDFLAGS = -m elf_x86_64 -T $(KERNEL)/kernel.ld -z max-page-size=4096 --defsym __BUILD_DATE=$(shell date +'%Y%m%d') --defsym __BUILD_TIME=$(shell date +'%H%M%S') -static -nostdlib

NAME = ToolOS
C_VERSION = c23 # c89, c99, c11, c17, c23

RM = rm -rf

SRC_C = $(wildcard $(KERNEL)/*.c)
SRC_S = $(wildcard $(KERNEL)/*.S)

OBJS = $(patsubst $(KERNEL)/%.c, $(TMP)/%.o, $(SRC_C))
OBJS += $(patsubst $(KERNEL)/%.S, $(TMP)/%.o, $(SRC_S))

all: $(NAME).elf

$(TMP):
	@mkdir -p $@

$(NAME).elf: $(OBJS)
	@$(LD) $(LDFLAGS) -o $@ $^

$(TMP)/%.o: $(KERNEL)/%.c | $(TMP)
	@echo [CC] $@
	@$(CC) $(CFLAGS) -std=$(C_VERSION) -c $< -o $@

$(TMP)/%.o: $(KERNEL)/%.S | $(TMP)
	@echo [ASM] $@
	@$(ASM) $(ASMFLAGS) $< -o $@

clean:
	$(RM) $(NAME).elf
	$(RM) $(TMP)
	@echo Cleaned.