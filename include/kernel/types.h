/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (c) 2026 Seonbin Yoon
*/

#ifndef TYPES_H
#define TYPES_H

#if defined(__x86_64__) || defined(__aarch64__)
    typedef long long s64;
    typedef int       s32;
    typedef short     s16;
    typedef char      s8;

    typedef unsigned long long u64;
    typedef unsigned int       u32;
    typedef unsigned short     u16;
    typedef unsigned char      u8;
#else
    #error "ToolOS only supports 64-bit architectures of compile environment (aarch64 || x86_64)."
#endif

#endif