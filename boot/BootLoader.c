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

typedef VOID (*GoToKernel)(TOOLOS_BOOTINFO_TABLE *BootInfo);

EFI_STATUS EFIAPI BootMain(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE * SystemTable) {
	EFI_STATUS Status;
	TOOLOS_BOOTINFO_TABLE *BootInfo = NULL;
	EFI_FILE_PROTOCOL *KernelFile = NULL;
	UINT64 KernelMemSize = 0;
	GoToKernel KernelEntry;

	SystemTable->ConOut->ClearScreen(SystemTable->ConOut);

	Status = CreateInfoTable(&BootInfo);
	if (EFI_ERROR(Status)) {
		Print(L"Failed Create_InfoTable. | Error code: %r", Status);
		CPU_HALT;
	}

	Status = GetACPIInfo(BootInfo);
	if (EFI_ERROR(Status)) {
		Print(L"Failed Get ACPI Info. | Error code: %r", Status);
		CPU_HALT;
	}

	Status = GetGOPInfo(BootInfo);
	if (EFI_ERROR(Status)) {
		Print(L"Failed Get GOP Info. | Error code: %r", Status);
		CPU_HALT;
	}

	Status = OpenKernelFile(ImageHandle, L"TOSKernel.elf", &KernelFile);
	if (EFI_ERROR(Status)) {
		Print(L"ERROR [A]");
		CPU_HALT;
	}

	Status = ValidationELFHeader(KernelFile, FALSE);
	if (EFI_ERROR(Status)) {
		Print(L"ERROR [B] | Error code: %r", Status);
		CPU_HALT;
	}

	Status = GetKernelFileSize(KernelFile, &KernelMemSize);
	if (EFI_ERROR(Status)) {
		Print(L"ERROR [C] | Error code: %r", Status);
		CPU_HALT;
	}

	Status = LoadKernelFile(BootInfo, KernelFile, 0x100000, KernelMemSize);
	if (EFI_ERROR(Status)) {
		Print(L"ERROR [D] | Error code: %r", Status);
		CPU_HALT;
	}

	Status = CloseKernelFile(KernelFile);
	if (EFI_ERROR(Status)) {
		Print(L"ERROR [E]");
		CPU_HALT;
	}

	Status = GetMemoryInfo(BootInfo);
	if (EFI_ERROR(Status)) {
		Print(L"ERROR [F] | Error code: %r", Status);
		CPU_HALT;
	}

	BootInfo->SafeWritten = TRUE;

	Status = gBS->ExitBootServices(
		ImageHandle,
		BootInfo->MemoryMapInfo.MapKey
	);
	if (EFI_ERROR(Status)) {
		Print(L"ERROR [G] | Error code: %r", Status);
		CPU_HALT;
	}

	KernelEntry = (GoToKernel)(BootInfo->KernelStartAddress);
	KernelEntry(BootInfo);

	return EFI_SUCCESS;
}
