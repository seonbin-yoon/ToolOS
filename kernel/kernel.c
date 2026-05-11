/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (c) 2026 Seonbin Yoon
 * Project: ToolOS
 * File: kernel.c
 * Description:
 *  
 */

#include "bootinfo.h"
#include "type.h"
#include "func.h"

void kernel_main(struct TOOLOS_MASTER_MAP *boot_info) {
	success_print(boot_info, false);
}

void success_print(struct TOOLOS_MASTER_MAP *boot_info, bool is_success) {
	u32 *gop = (u32*)boot_info->GraphicsMap.FrameBufferBase;
	if (is_success) {
		for (u64 i = 0; i < boot_info->GraphicsMap.PixelsPerScanLine * 5; i++) {
			gop[i] = 0x00BFFF;
		}
	} else {
		for (u64 i = 0; i < boot_info->GraphicsMap.PixelsPerScanLine * 5; i++) {
			gop[i] = 0xFF0000;
		}
	}
}
