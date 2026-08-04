/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (c) 2026 Seonbin Yoon
 * Project: ToolOS
 * File: boot.h
 * Description:
 *  
 */

#ifndef BOOT_H
#define BOOT_H

#include <bootstruct.h>

void success_print(struct toolos_graphics_map *graphics_map, bool is_success);
i32 set_paging();
i32 set_gdt();

#endif
