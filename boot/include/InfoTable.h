/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (c) 2026 Seonbin Yoon
 * Project: ToolOS
 * File: InfoTable.c
 * Description:
 *   Where the structure that the kernel reads is defined
 */

#ifndef InfoTable_h
#define InfoTable_h

// Using For ToolOS

#include "TBL.h"

#pragma pack(1)
// 8 + 1 + 6 + 1 + 4 + 4 + 8 + 1 + 3 = 36byte
typedef struct {
	CHAR8  Signature[8];
	UINT8  Checksum;
	CHAR8  OEMID[6];
	UINT8  Revision;
	UINT32 RsdtAddress;
	UINT32 Length;
	UINT64 XsdtAddress;
	UINT8  ExtendedChecksum;
	UINT8  Reserved[3];
} TOOLOS_ACPI_TABLE;
#pragma pack()

#pragma pack(1)
// 4 + 4 + 8 + 8 = 24byte
typedef struct {
	UINT32                  Type;
	UINT32                  Reserved; // MUST BE 0
	UINT64                  PhysicalStart;
	UINT64                  NumberOfPages;
} TOOLOS_MEMORY_MAP;
#pragma pack()

#pragma pack(1)
// 8 + 8 + 8 = 24byte
typedef struct {
	UINTN MemoryMapNums;
	UINTN MapKey;
	UINTN TotalMemorySize;
} TOOLOS_MEMORY_MAPINFO;

#pragma pack(1)
// 4 + 4 + 4 + 4 = 16byte
typedef struct {
	UINT32 RedMask;
	UINT32 GreenMask;
	UINT32 BlueMask;
	UINT32 ReservedMask;
} TOOLOS_PIXEL_BITMASK;
#pragma pack()

#pragma pack(1)
// 8 + 8 + 4 + 4 + 4 + 4 + 16 = 48byte
typedef struct {
	EFI_PHYSICAL_ADDRESS    FrameBufferBase;
	UINTN                   FrameBufferSize;
	UINT32                  Version;
	UINT32                  HorizontalResolution;
	UINT32                  VerticalResolution;
	UINT32                  PixelsPerScanLine;
	TOOLOS_PIXEL_BITMASK    PixelInformation;
} TOOLOS_GRAPHICS_MAP;
#pragma pack()

// 16 + 48 + 8 + 24 + 8 + 1 + 7 = 112byte
#pragma pack(1)
typedef struct {
	CHAR8                    Signature[16];
	UINT64                   KernelStartAddress;
	TOOLOS_GRAPHICS_MAP      T_GraphicsMap;
	TOOLOS_ACPI_TABLE*       T_ACPITable;
	TOOLOS_MEMORY_MAPINFO    T_MemoryMapInfo;
	TOOLOS_MEMORY_MAP*       T_MemoryMap;
	CHAR8                    CheckSum;
	UINT8                    Reserved[7];
} TOOLOS_MASTER_MAP;
#pragma pack()

#endif


