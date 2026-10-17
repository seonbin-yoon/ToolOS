/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (c) 2026 Seonbin Yoon
*/

#ifndef AARCH64_H
#define AARCH64_H

static inline void death() {
    do {
        __asm__ __volatile__ ("msr daifset, #0xf\n 1:\n\t wfe\n\t b 1b");
    } while (false);
}

#endif