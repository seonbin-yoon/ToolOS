/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (c) 2026 Seonbin Yoon
 * Project: ToolOS
 * File: BootLoader.c
 * Description:
 *   This is the main bootloader for ToolOS
 */

#include "TBL.h"

EFI_STATUS EFIAPI BootMain(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE * SystemTable) {
	SystemTable->ConOut->ClearScreen(SystemTable->ConOut);

	EFI_STATUS Status;
	TOOLOS_MASTER_MAP *BootInfo = NULL;
	EFI_FILE_PROTOCOL *KernelFile = NULL;
	/* GoToKernel JumpToKernel; */

	Status = Create_InfoTable(&BootInfo);
	if (EFI_ERROR(Status)) {
		Print(L"Failed Create_InfoTable. | Error code: %r", Status);
		CPU_HALT;
	}

	Status = Get_ACPI_Info(BootInfo);
	if (EFI_ERROR(Status)) {
		Print(L"Failed Get ACPI Info. | Error code: %r", Status);
		CPU_HALT;
	}

	Status = Get_GOP_Info(BootInfo);
	if (EFI_ERROR(Status)) {
		Print(L"Failed Get GOP Info. | Error code: %r", Status);
		CPU_HALT;
	}

	Status = OpenKernelFile(ImageHandle, L"TOSKernel.elf", &KernelFile);
	if (EFI_ERROR(Status)) {
		Print(L"Failed Open to Kernel File. | Error code: %r", Status);
		CPU_HALT;
	}

	return EFI_SUCCESS;
}	
