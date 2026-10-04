/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (c) 2026 Seonbin Yoon
*/

#define CPU_HALT \
	do { __asm__ volatile ("msr daifset, #0xf\n 1:\n\t wfi\n\t b 1b"); } while (false)