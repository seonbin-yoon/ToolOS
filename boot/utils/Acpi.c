/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (c) 2026 Seonbin Yoon
 * Project: ToolOS
 * File: Acpi.c
 * Description:
 *   Checks each system table to determine if it is an ACPI table; if no tables are found,
 *   it immediately handles the exception, and if one is found, it records the address in InfoTable.
 */

#include "TBL.h"

const CHAR8 ACPI_TABLE_Signature[8] = {'R', 'S', 'D', ' ', 'P', 'T', 'R', ' '};

EFI_STATUS GetACPIInfo(IN TOOLOS_MASTER_MAP *BootInfo) {
	EFI_STATUS Status;

	if (BootInfo == NULL || CompareMem(BootInfo->Signature, TOOLOS_INFOTABLE_Signature, 16) != 0) {
		Status = EFI_INVALID_PARAMETER;
		goto out;
	}

	for (UINTN i = 0; i < gST->NumberOfTableEntries; i++) {
		if (CompareGuid(&gST->ConfigurationTable[i].VendorGuid, &gEfiAcpi20TableGuid)) {
			BootInfo->T_ACPITable = (TOOLOS_ACPI_TABLE *)gST->ConfigurationTable[i].VendorTable;
			Status = EFI_SUCCESS;
			goto out;
		}
	}

	Status = EFI_NOT_FOUND;

out:
	return Status;
}
