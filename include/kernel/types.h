/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (c) 2026 Seonbin Yoon
*/

#ifndef TYPES_H
#define TYPES_H

#if defined(__x86_64__) || defined(__aarch64__)
    typedef long long size64;
    typedef int       size32;
    typedef short     size16;
    typedef char      size8;
#endif

#endif