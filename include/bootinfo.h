/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (c) 2026 Seonbin Yoon
 * Project: ToolOS
 * File: bootinfo.h
 * Description:
 *  
 */

#ifndef Bootinfo_h
#define Bootinfo_h

#include "type.h"

#pragma pack(1)
// 8 + 1 + 6 + 1 + 4 + 4 + 8 + 1 + 3 = 36byte
struct TOOLOS_ACPI_TABLE {
	c8  Signature[8];
	u8  Checksum;
	c8  OEMID[6];
	u8  Revision;
	u32 RsdtAddress;
	u32 Length;
	u64 XsdtAddress;
	u8  ExtendedChecksum;
	u8  Reserved[3];
};
#pragma pack()

#pragma pack(1)
// 4 + 4 + 8 + 8 = 24byte
struct TOOLOS_MEMORY_MAP {
	u32 Type;
	u32 Reserved; // MUST BE 0
	u64 PhysicalStart;
	u64 NumberOfPages;
};
#pragma pack()

#pragma pack(1)
// 8 + 8 + 8 = 24byte
struct TOOLOS_MEMORY_MAPINFO {
	u64 MemoryMapNums;
	u64 MapKey;
	u64 TotalMemorySize;
};
#pragma pack()

#pragma pack(1)
// 4 + 4 + 4 + 4 = 16byte
struct TOOLOS_PIXEL_BITMASK {
	u32 RedMask;
	u32 GreenMask;
	u32 BlueMask;
	u32 ReservedMask;
};
#pragma pack()

#pragma pack(1)
// 8 + 8 + 4 + 4 + 4 + 4 + 16 = 48byte
struct TOOLOS_GRAPHICS_MAP {
	u64                         FrameBufferBase;
	u64                         FrameBufferSize;
	u32                         Version;
	u32                         HorizontalResolution;
	u32                         VerticalResolution;
	u32                         PixelsPerScanLine;
	struct TOOLOS_PIXEL_BITMASK PixelInformation;
};
#pragma pack()

// 16 + 8 + 24 + 8 + 48 + 8 + 1 + 7 = 120byte
#pragma pack(1)
struct TOOLOS_MASTER_MAP {
	c8                           Signature[16];
	u64                          KernelStartAddress;
	struct TOOLOS_MEMORY_MAPINFO MemoryMapInfo;
	struct TOOLOS_MEMORY_MAP     *MemoryMap;
	struct TOOLOS_GRAPHICS_MAP   GraphicsMap;
	struct TOOLOS_ACPI_TABLE     *ACPITable;
	c8                           CheckSum;
	u8                           Reserved[7];
};
#pragma pack()

#endif
