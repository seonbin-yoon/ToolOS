CC = clang
ASM = clang
LD = ld.lld

KERNEL = kernel
LDSCRIPT = $(KERNEL)/kernel.ld
INCLUDE = include
TMP = build

NAME = ToolOS
C_STD = c23

RM = rm -rf

ASMFLAGS = --target=aarch64-none-elf -g -MMD -MP
CFLAGS = --target=aarch64-none-elf -g -O2 -Wall -Werror -ffreestanding \
		-finline-functions -fno-stack-protector -nostdlibinc -std=$(C_STD) \
		-I$(INCLUDE) -MMD -MP
LDFLAGS = -m aarch64elf -T $(LDSCRIPT) \
		-static -nostdlib -z max-page-size=4096 \
		--defsym __BUILD_DATE=$(shell date +'%Y%m%d') \
		--defsym __BUILD_TIME=$(shell date +'%H%M%S')

CFILES = $(wildcard $(KERNEL)/*.c)
ASMFILES = $(wildcard $(KERNEL)/*.S)

OBJS = $(patsubst $(KERNEL)/%.c, $(TMP)/%.o, $(CFILES))
OBJS += $(patsubst $(KERNEL)/%.S, $(TMP)/%.o, $(ASMFILES))

$(NAME).elf: $(OBJS) $(LDSCRIPT)
	@echo [LD] $@
	@$(LD) $(LDFLAGS) -o $@ $(OBJS)

$(TMP):
	@mkdir -p $@

$(TMP)/%.o: $(KERNEL)/%.c | $(TMP)
	@echo [CC] $@
	@$(CC) $(CFLAGS) -c $< -o $@

$(TMP)/%.o: $(KERNEL)/%.S | $(TMP)
	@echo [ASM] $@
	@$(ASM) $(ASMFLAGS) -c $< -o $@

clean:
	$(RM) $(NAME).elf $(TMP)
	@echo Cleaned.

-include $(OBJS:.o=.d)

.PHONY: clean