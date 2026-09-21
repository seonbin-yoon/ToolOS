/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (c) 2026 Seonbin Yoon
 * Project: ToolOS
*/

#ifndef MAIN_H
#define MAIN_H

#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Guid/Acpi.h>
#include <Guid/FileInfo.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/LoadedImage.h>

#include <bootinfo.h>
#include <ELF.h>

#define CPU_HALT \
	do { __asm__ volatile ("msr daifset, #0xf\n 1:\n\t wfi\n\t b 1b"); } while (FALSE)

typedef VOID (*GoToKernel)(TOOLOS_BOOTINFO_TABLE *BootInfo);

EFI_STATUS
(CreateInfoTable)(
	TOOLOS_BOOTINFO_TABLE **buffer
);

EFI_STATUS
(GetMemoryInfo)(
	TOOLOS_BOOTINFO_TABLE *BootInfo
);

EFI_STATUS
(GetACPIInfo)(
	TOOLOS_BOOTINFO_TABLE *BootInfo
);

EFI_STATUS
(GetGOPInfo)(
	TOOLOS_BOOTINFO_TABLE *BootInfo
);

EFI_STATUS
(OpenKernelFile)(
	EFI_HANDLE BootLoaderHandle,
	CHAR16 *FileName,
	EFI_FILE_PROTOCOL **File
);

EFI_STATUS
(ValidationKernelFile)(
	EFI_FILE_PROTOCOL *File
);

EFI_STATUS
(GetKernelFileSize)(
	EFI_FILE_PROTOCOL *File,
	UINT64 *SizeBuffer
);

EFI_STATUS
(LoadKernelFile)(
	TOOLOS_BOOTINFO_TABLE *BootInfo,
	EFI_FILE_PROTOCOL *File,
	EFI_PHYSICAL_ADDRESS LoadAddress,
	UINT64 Size
);

EFI_STATUS
(CloseKernelFile)(
	EFI_FILE_PROTOCOL *File
);

extern const CHAR8 BootInfo_Signature[16];

#endif
