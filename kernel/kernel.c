/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (c) 2026 Seonbin Yoon
 * Project: ToolOS
 * File: kernel.c
 * Description:
 *  
 */

#include <boot.h>
#include <macros.h>

void kernel_main(struct toolos_bootinfo_table *boot_info) {
	i32 ret_code;

	ret_code = set_gdt();
	if (ret_code)
		success_print(&boot_info->graphics_map, false);

	HALT;
}

i32 set_gdt() {
	i32 ret_code = 0;

	return ret_code;
}

i32 set_paging() {
	i32 ret_code = 0;

	return ret_code;
}

void success_print(struct toolos_graphics_map *graphics_map, bool is_success) {
	u32 *gop = (u32 *)graphics_map->frame_buffer_base;
	u32 color = is_success ? 0x00BFFF : 0xFF0000;

	for (u64 i = 0; i < graphics_map->pixels_per_scan_line * 5; i++)
		gop[i] = color;
}
