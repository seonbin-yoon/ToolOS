/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (c) 2026 Seonbin Yoon
 * Project: ToolOS
 * File: KernelLoad.c
 * Description:
 *   It is responsible for kernel loading, addressing, and load preparation.
 */

#include "TBL.h"

#define LOADIMAGE_GUID gEfiLoadedImageProtocolGuid
#define FileSystem_GUID gEfiSimpleFileSystemProtocolGuid
#define _1MB 1048576
