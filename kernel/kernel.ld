/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (c) 2026 Seonbin Yoon
 * Project: ToolOS
 * File: kernel.ld
 * Description:
 *  
 */

OUTPUT_FORMAT("elf64-x86-64")
OUTPUT_ARCH("i386:x86-64")
ENTRY(_start)

SECTIONS
{
	. = 0x100000;

	.text BLOCK(0x1000) : {
		*(.text)
	}

	.rodata BLOCK(0x1000) : {
		*(.rodata)
	}

	.data BLOCK(0x1000) : {
		*(.data)
	}

	.bss BLOCK(0x1000) : {
		*(.bss)
	}
}
