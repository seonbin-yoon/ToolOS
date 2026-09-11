/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (c) 2026 Seonbin Yoon
 * Project: ToolOS
*/

#include <main.h>

const CHAR8 BootInfo_Signature[16] = {"TOOLOS BOOTINFO"};

EFI_STATUS CreateInfoTable(TOOLOS_BOOTINFO_TABLE **buffer) {
	EFI_STATUS Status;
	UINT64 TableSize = sizeof(TOOLOS_BOOTINFO_TABLE);
	TOOLOS_BOOTINFO_TABLE *Temp = NULL;

	if (buffer == NULL) {
		Status = EFI_INVALID_PARAMETER;
		goto out;
	}

	Status = gBS->AllocatePool(
		EfiLoaderData,
		TableSize,
		(VOID **)&Temp
	);
	if (EFI_ERROR(Status))
		goto out;

	gBS->SetMem(
		Temp,
		TableSize,
		0
	);

	for (UINTN i = 0; i < 16; i++)
		Temp->Signature[i] = BootInfo_Signature[i];

	*buffer = Temp;

out:
	return Status;
}
