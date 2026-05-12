/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (c) 2026 Seonbin Yoon
 * Project: ToolOS
 * File: ELF.h
 * Description:
 *   Where the standard ELF header is defined
 */

#ifndef ELF_H
#define ELF_H

#include "TBL.h"
// 16 + 2 + 2 + 4 + 8 + 8 + 8 + 4 + 2 + 2 + 2 + 2 + 2 + 2 = 64byte
// ELF Header
#pragma pack(1)
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
#pragma pack()

// 4 + 4 + 8 + 8 + 8 + 8 + 8 + 8 = 56byte
// ELF Program Header
#pragma pack(1)
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
