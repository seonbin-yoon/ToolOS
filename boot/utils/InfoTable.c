/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (c) 2026 Seonbin Yoon
 * Project: ToolOS
 * File: InfoTable.c
 * Description:
 *   Creates and returns a structure that the kernel can read.
 */

#include "TBL.h"

const CHAR8 TOOLOS_INFOTABLE_Signature[16] = {"TOOLOS BOOTINFO"};

EFI_STATUS Create_InfoTable(IN OUT TOOLOS_MASTER_MAP **Table_Pointer) {
	if (Table_Pointer == NULL)
		return EFI_INVALID_PARAMETER;
	
	EFI_STATUS Status;
	UINT64 TableSize = sizeof(TOOLOS_MASTER_MAP);
	TOOLOS_MASTER_MAP *TempTable = NULL;

	Status = gBS->AllocatePool(
		EfiLoaderData,
		TableSize,
		(VOID **)&TempTable
	);

	if (EFI_ERROR(Status))
		return Status;

	gBS->SetMem(
		TempTable,
		TableSize,
		0
	);

	for (UINTN i = 0; i < 16; i++)
		TempTable->Signature[i] = TOOLOS_INFOTABLE_Signature[i];

	*Table_Pointer = TempTable;

	return EFI_SUCCESS;
}
