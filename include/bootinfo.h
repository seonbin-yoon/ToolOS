/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (c) 2026 Seonbin Yoon
 * Project: ToolOS
 * File: bootinfo.h
 * Description:
 *  
 */

#ifndef BOOTINFO_H
#define BOOTINFO_H

#include <types.h>

#pragma pack(1)
// 8 + 1 + 6 + 1 + 4 + 4 + 8 + 1 + 3 = 36byte
struct acpi_rsdp_table {
	c8  signature[8];
	u8  checksum;
	c8  oemid[6];
	u8  revision;
	u32 rsdt_address;
	u32 length;
	u64 xsdt_address;
	u8  extended_checksum;
	u8  reserved[3];
};
#pragma pack()

#pragma pack(1)
// 8 + 8 + 8 + 8 = 32byte
struct toolos_memory_map {
	u64 type;
	u64 physical_start;
	u64 number_of_pages;
	u64 attribute;
};
#pragma pack()

#pragma pack(1)
// 8 + 8 + 8 = 24byte
struct toolos_memory_mapinfo {
	u64 memory_map_nums;
	u64 mapkey;
	u64 total_memory_size;
};

#pragma pack(1)
// 4 + 4 + 4 + 4 = 16byte
struct toolos_pixel_bitmask {
	u32 red_mask;
	u32 green_mask;
	u32 blue_mask;
	u32 reserved_mask;
};
#pragma pack()

#pragma pack(1)
// 8 + 8 + 4 + 4 + 4 + 4 + 16 = 48byte
struct toolos_graphics_map {
	u64                         frame_buffer_base;
	u64                         frame_buffer_size;
	u32                         version;
	u32                         horizontal_resolution;
	u32                         vertical_resolution;
	u32                         pixels_per_scan_line;
	struct toolos_pixel_bitmask pixel_Information;
};
#pragma pack()

// 16 + 8 + 24 + 8 + 48 + 8 + 1 + 7 = 120byte
#pragma pack(1)
struct toolos_bootinfo_table {
	c8                           signature[16];
	u64                          kernel_start_Address;
	struct toolos_memory_mapinfo memory_mapinfo;
	struct toolos_memory_map*    memory_map;
	struct toolos_graphics_map   graphics_map;
	struct acpi_rsdp_table*      rsdp_table;
	u8                           reserved[7];
	bool                         safe_written;
};
#pragma pack()

#endif
