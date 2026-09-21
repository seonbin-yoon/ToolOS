/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (c) 2026 Seonbin Yoon
 * Project: ToolOS
*/

#ifndef ELF_H
#define ELF_H

#include <Uefi.h>

#define ELF_MAGIC_NUM_LEN 4
#define FOR_64BIT 0x2
#define LITTLE_ENDIAN 0x01
#define EXEC    0x2
#define AARCH64 0xB7
#define PT_LOAD 0x1

#pragma pack(1)

// 16 + 2 + 2 + 4 + 8 + 8 + 8 + 4 + 2 + 2 + 2 + 2 + 2 + 2 = 64byte
typedef struct {
	CHAR8  e_ident[16];
	UINT16 e_type;
	UINT16 e_machine;
	UINT32 e_version;
	UINT64 e_entry;
	UINT64 e_phoff;
	UINT64 e_shoff;
	UINT32 e_flags;
	UINT16 e_ehsize;
	UINT16 e_phentsize;
	UINT16 e_phnum;
	UINT16 e_shentsize;
	UINT16 e_shnum;
	UINT16 e_shstrndx;
} ELFHeader;

// 4 + 4 + 8 + 8 + 8 + 8 + 8 + 8 = 56byte
typedef struct {
	UINT32 p_type;
	UINT32 p_flags;
	UINT64 p_offset;
	UINT64 p_vaddr;
	UINT64 p_paddr;
	UINT64 p_filesz;
	UINT64 p_memsz;
	UINT64 p_align;
} ELFProgramHeader;

#pragma pack()
#endif
