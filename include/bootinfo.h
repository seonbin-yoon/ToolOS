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
typedef struct {
	c8  Signature[8];
	u8  Checksum;
	c8  OEMID[6];
	u8  Revision;
	u32 RsdtAddress;
	u32 Length;
	u64 XsdtAddress;
	u8  ExtendedChecksum;
	u8  Reserved[3];
} ACPI_RSDP_TABLE;
#pragma pack()

#pragma pack(1)
// 8 + 8 + 8 + 8 = 32byte
typedef struct {
	u64 Type;
	u64 PhysicalStart;
	u64 NumberOfPages;
	u64 Attribute;
} TOOLOS_MEMORY_MAP;
#pragma pack()

#pragma pack(1)
// 8 + 8 + 8 = 24byte
typedef struct {
	u64 MemoryMapNums;
	u64 MapKey;
	u64 TotalMemorySize;
} TOOLOS_MEMORY_MAPINFO;

#pragma pack(1)
// 4 + 4 + 4 + 4 = 16byte
typedef struct {
	u32 RedMask;
	u32 GreenMask;
	u32 BlueMask;
	u32 ReservedMask;
} TOOLOS_PIXEL_BITMASK;
#pragma pack()

#pragma pack(1)
// 8 + 8 + 4 + 4 + 4 + 4 + 16 = 48byte
typedef struct {
	u64                  FrameBufferBase;
	u64                  FrameBufferSize;
	u32                  Version;
	u32                  HorizontalResolution;
	u32                  VerticalResolution;
	u32                  PixelsPerScanLine;
	TOOLOS_PIXEL_BITMASK PixelInformation;
} TOOLOS_GRAPHICS_MAP;
#pragma pack()

// 16 + 8 + 24 + 8 + 48 + 8 + 1 + 7 = 128byte
#pragma pack(1)
typedef struct {
	c8                    Signature[16];
	u64                   KernelStartAddress;
	TOOLOS_MEMORY_MAPINFO MemoryMapInfo;
	TOOLOS_MEMORY_MAP*    MemoryMap;
	TOOLOS_GRAPHICS_MAP   GraphicsMap;
	ACPI_RSDP_TABLE*      RSDPTable;
	u8                    Reserved[7];
	bool                  SafeWritten;
} TOOLOS_BOOTINFO_TABLE;
#pragma pack()

#endif
