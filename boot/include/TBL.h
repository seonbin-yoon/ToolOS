/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (c) 2026 Seonbin Yoon
 * Project: ToolOS
 * File: TBL.h
 * Description:
 *   The function definitions for all utility C files are located here.
 */

#ifndef TBL_H
#define TBL_H

#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Guid/Acpi.h>
#include <Guid/FileInfo.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/LoadedImage.h>
#include "InfoTable.h"
#include "ELF.h"

#define CPU_HALT \
	do { __asm__ __volatile__ ("cli; hlt"); } while (0)

EFI_STATUS
(Create_InfoTable)(
	IN OUT TOOLOS_MASTER_MAP **Table_Pointer
);

EFI_STATUS
(GetMemoryInfo)(
	IN TOOLOS_MASTER_MAP *BootInfo
);

EFI_STATUS
(GetACPIInfo)(
	IN TOOLOS_MASTER_MAP *BootInfo
);

EFI_STATUS
(Get_GOP_Info)(
	IN TOOLOS_MASTER_MAP *BootInfo
);

EFI_STATUS
(OpenKernelFile)(
	IN EFI_HANDLE BootLoaderHandle,
	IN CHAR16 *FileName,
	IN OUT EFI_FILE_PROTOCOL **File
);

EFI_STATUS
(ValidationELFHeader)(
	IN EFI_FILE_PROTOCOL *File,
	IN BOOLEAN IsBigEndian
);

extern const CHAR8 TOOLOS_INFOTABLE_Signature[16];

#endif
