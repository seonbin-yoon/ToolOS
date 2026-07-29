/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (c) 2026 Seonbin Yoon
 * Project: ToolOS
 * File: func.h
 * Description:
 *  
 */

#ifndef FUNC_H
#define FUNC_H

#include <bootstruct.h>

void success_print(struct toolos_graphics_map *graphics_map, bool is_success);
int set_paging();
int set_gdt();

#endif
