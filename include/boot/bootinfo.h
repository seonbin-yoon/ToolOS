/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (c) 2026 Seonbin Yoon
*/

#ifndef BOOTINFO_H
#define BOOTINFO_H

#pragma pack(1)

#if defined(__x86_64__) || defined(__aarch64__)
	// 8 + 1 + 6 + 1 + 4 + 4 + 8 + 1 + 3 = 36 byte
	typedef struct {
		unsigned char      Signature[8];
		unsigned char      Checksum;
		unsigned char      OemId[6];
		unsigned char      Revision;
		unsigned int       RsdtAddress;
		unsigned int       Length;
		unsigned long long XsdtAddress;
		unsigned char      ExtendedChecksum;
		unsigned char      Reserved[3];
	} ACPI_RSDP_TABLE;

	// 8 + 8 + 8 + 8 = 32byte
	typedef struct {
		unsigned long long Type;
		unsigned long long PhysicalStart;
		unsigned long long NumberOfPages;
		unsigned long long Attribute;
	} TOOLOS_MEMORY_MAP;

	// 8 + 8 + 8 = 24byte
	typedef struct {
		unsigned long long MemoryMapNums;
		unsigned long long MapKey;
		unsigned long long TotalMemorySize;
	} TOOLOS_MEMORY_MAPINFO;


	// 4 + 4 + 4 + 4 = 16byte
	typedef struct {
		unsigned int RedMask;
		unsigned int GreenMask;
		unsigned int BlueMask;
		unsigned int ReservedMask;
	} TOOLOS_PIXEL_BITMASK;

	// 8 + 8 + 4 + 4 + 4 + 4 + 16 = 48byte
	typedef struct {
		unsigned long long   FrameBufferBase;
		unsigned long long   FrameBufferSize;
		unsigned int         Version;
		unsigned int         HorizontalResolution;
		unsigned int         VerticalResolution;
		unsigned int         PixelsPerScanLine;
		TOOLOS_PIXEL_BITMASK PixelInformation;
	} TOOLOS_GRAPHICS_MAP;

	// 16 + 8 + 24 + 8 + 48 + 8 = 112byte
	typedef struct {
		unsigned char           Signature[16];
		unsigned long long    KernelStartAddress;
		TOOLOS_MEMORY_MAPINFO MemoryMapInfo;
		TOOLOS_MEMORY_MAP     *MemoryMap;
		TOOLOS_GRAPHICS_MAP   GraphicsMap;
		ACPI_RSDP_TABLE       *RSDPTable;
	} TOOLOS_BOOTINFO_TABLE;
#else
    #error "ToolOS only supports 64-bit architectures of compile environment (aarch64 || x86_64)."
#endif

#pragma pack()
#endif
