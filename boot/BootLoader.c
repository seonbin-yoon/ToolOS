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

EFI_STATUS EFIAPI BootMain(EFI_HANDLE ThisHandle, EFI_SYSTEM_TABLE * SystemTable) {
	SystemTable->ConOut->ClearScreen(SystemTable->ConOut);

	EFI_STATUS Status;
	TOOLOS_MASTER_MAP *BootInfo = NULL;
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

	UINT32 *Pointer = (UINT32 *)BootInfo->T_GraphicsMap.FrameBufferBase;

	for (UINTN i = 0; i < BootInfo->T_GraphicsMap.PixelsPerScanLine * 5; i++) {
		Pointer[i] = 0x7F;
	}

	Print(L"ACPI RSDP : %p, Frame Buffer : %p\r\nGreat!\r\n", BootInfo->T_ACPITable, BootInfo->T_GraphicsMap.FrameBufferBase);
	Print(L"The system will halt now!\r\n");
	CPU_HALT;

	return EFI_SUCCESS;
}
