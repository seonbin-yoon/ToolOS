/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (c) 2026 Seonbin Yoon
 * Project: ToolOS
 * File: macro.h
 * Description:
 *  
 */

#ifndef MACRO_H
#define MACRO_H

#define HALT do { __asm__ __volatile__ ("hlt"); } while(false)
#define INSERT_REG_R15(x) do { __asm__ __volatile__ ("mov r15, %0": : "r"(x)); } while(false)

#endif