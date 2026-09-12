/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (c) 2026 Seonbin Yoon
 * Project: ToolOS
*/

#ifndef BOOTINFO_H
#define BOOTINFO_H

#include <Uefi.h>
#pragma pack(1)

// 8 + 1 + 6 + 1 + 4 + 4 + 8 + 1 + 3 = 36 byte
typedef struct {
	UINT64    Signature;
	UINT8     Checksum;
	UINT8     OemId[6];
	UINT8     Revision;
	UINT32    RsdtAddress;
	UINT32    Length;
	UINT64    XsdtAddress;
	UINT8     ExtendedChecksum;
	UINT8     Reserved[3];
} RSDP_TABLE;

// 8 + 8 + 8 + 8 = 32byte
typedef struct {
	UINT64 Type;
	UINT64 PhysicalStart;
	UINT64 NumberOfPages;
	UINT64 Attribute;
} TOOLOS_MEMORY_MAP;

// 8 + 8 + 8 = 24byte
typedef struct {
	UINT64 MemoryMapNums;
	UINT64 MapKey;
	UINT64 TotalMemorySize;
} TOOLOS_MEMORY_MAPINFO;


// 4 + 4 + 4 + 4 = 16byte
typedef struct {
	UINT32 RedMask;
	UINT32 GreenMask;
	UINT32 BlueMask;
	UINT32 ReservedMask;
} TOOLOS_PIXEL_BITMASK;

// 8 + 8 + 4 + 4 + 4 + 4 + 16 = 48byte
typedef struct {
	EFI_PHYSICAL_ADDRESS    FrameBufferBase;
	UINT64                  FrameBufferSize;
	UINT32                  Version;
	UINT32                  HorizontalResolution;
	UINT32                  VerticalResolution;
	UINT32                  PixelsPerScanLine;
	TOOLOS_PIXEL_BITMASK    PixelInformation;
} TOOLOS_GRAPHICS_MAP;

// 16 + 8 + 24 + 8 + 48 + 8 + 7 + 1 = 120byte
typedef struct {
	CHAR8                 Signature[16];
	EFI_PHYSICAL_ADDRESS  KernelStartAddress;
	TOOLOS_MEMORY_MAPINFO MemoryMapInfo;
	TOOLOS_MEMORY_MAP*    MemoryMap;
	TOOLOS_GRAPHICS_MAP   GraphicsMap;
	RSDP_TABLE*           RSDPTable;
	UINT8                 Reserved[7];
	BOOLEAN               SafeWritten;
} TOOLOS_BOOTINFO_TABLE;

#pragma pack()
#endif
