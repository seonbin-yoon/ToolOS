/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (c) 2026 Seonbin Yoon
 * Project: ToolOS
*/

.section .text
.global _entry

#define success #0x1234

_entry:
    mov x0, success
    b wait

wait:
    wfi
    b wait
