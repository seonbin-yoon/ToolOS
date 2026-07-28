/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (c) 2026 Seonbin Yoon
 * Project: ToolOS
 * File: kernel.c
 * Description:
 *  
 */

#include <bootinfo.h>
#include <types.h>
#include <func.h>

void kernel_main(struct toolos_bootinfo_table *boot_info) {
	success_print(boot_info, true);
}

void success_print(struct toolos_bootinfo_table *boot_info, bool is_success) {
	u32 *gop = (u32 *)boot_info->graphics_map.frame_buffer_base;

	if (is_success) {
		for (u64 i = 0; i < boot_info->graphics_map.pixels_per_scan_line * 5; i++)
			gop[i] = 0x00BFFF;
	} else {
		for (u64 i = 0; i < boot_info->graphics_map.pixels_per_scan_line * 5; i++)
			gop[i] = 0xFF0000;
	}
}
