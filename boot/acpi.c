/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (c) 2026 Seonbin Yoon
 * Project: ToolOS
*/

#include <main.h>

EFI_STATUS GetACPIInfo(TOOLOS_BOOTINFO_TABLE *BootInfo) {
	EFI_STATUS Status;

	if (!BootInfo || CompareMem(BootInfo->Signature, BootInfo_Signature, 16)) {
		Status = EFI_INVALID_PARAMETER;
		goto out;
	}

	for (UINTN i = 0; i < gST->NumberOfTableEntries; i++) {
		if (CompareGuid(&gST->ConfigurationTable[i].VendorGuid, &gEfiAcpi20TableGuid)) {
			BootInfo->RSDPTable = (ACPI_RSDP_TABLE *)gST->ConfigurationTable[i].VendorTable;
			Status = EFI_SUCCESS;
			goto out;
		}
	}

	Status = EFI_NOT_FOUND;

out:
	return Status;
}
