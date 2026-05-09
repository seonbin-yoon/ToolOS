/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (c) 2026 Seonbin Yoon
 * Project: ToolOS
 * File: Memory.c
 * Description:
 *   It provides a function to load a custom memory map. Internally, it calls the GetMemoryMap function, 
 *   records the address in the BootInfo table provided by the caller, and then returns.
 */

#include "TBL.h"

void* Last_MemoryMapAddress = NULL;
void* Last_ToolOSMemoryMapAddress = NULL;

EFI_STATUS Get_EFI_MemoryMap(IN TOOLOS_MASTER_MAP* BootInfo) {
	EFI_STATUS Status;
	UINTN MemoryMapSize = 0;
	EFI_MEMORY_DESCRIPTOR* MemoryMap = NULL;
	UINTN MapKey = 0;
	UINTN DescriptorSize = 0;
	UINT32 DescriptorVersion = 0;
	UINTN TOOLOSMemoryMapSize;
	EFI_MEMORY_DESCRIPTOR* MPTR;

	if (BootInfo == NULL || CompareMem(BootInfo->Signature, TOOLOS_INFOTABLE_Signature, 16) != 0) {
		Status = EFI_INVALID_PARAMETER;
		goto out;
	}

	if (Last_MemoryMapAddress || Last_ToolOSMemoryMapAddress) {
		gBS->FreePool(Last_MemoryMapAddress);
		gBS->FreePool(Last_ToolOSMemoryMapAddress);
		Last_MemoryMapAddress = NULL;
		Last_ToolOSMemoryMapAddress = NULL;
	}

	Status = gBS->GetMemoryMap(
		&MemoryMapSize,
		NULL,
		&MapKey,
		&DescriptorSize,
		&DescriptorVersion
	);

	if (Status != EFI_BUFFER_TOO_SMALL) {
		goto out;
	}

	MemoryMapSize += DescriptorSize * 3;
	BootInfo->T_MemoryMapInfo.MemoryMapNums = MemoryMapSize / DescriptorSize;
	TOOLOSMemoryMapSize = sizeof(TOOLOS_MEMORY_MAP) * BootInfo->T_MemoryMapInfo.MemoryMapNums;

	Status = gBS->AllocatePool(
		EfiLoaderData,
		TOOLOSMemoryMapSize,
		(VOID **)&BootInfo->T_MemoryMap
	);

	if (EFI_ERROR(Status)) {
		goto out;
	}

	Status = gBS->AllocatePool(
		EfiLoaderData,
		MemoryMapSize,
		(VOID **)&MemoryMap
	);

	if (EFI_ERROR(Status)) {
		goto out;
	}

	Status = gBS->GetMemoryMap(
		&MemoryMapSize,
		MemoryMap,
		&MapKey,
		&DescriptorSize,
		&DescriptorVersion
	);
	
	if (EFI_ERROR(Status)) {
		goto out;
	}

	MPTR = MemoryMap;
	if (BootInfo->T_MemoryMapInfo.MemoryMapNums < MemoryMapSize / DescriptorSize) {
		goto out;
	}

	BootInfo->T_MemoryMapInfo.MemoryMapNums = MemoryMapSize / DescriptorSize;

	for (UINTN i = 0; i < BootInfo->T_MemoryMapInfo.MemoryMapNums; i++) {
		BootInfo->T_MemoryMap[i].Type = MPTR->Type;
		BootInfo->T_MemoryMap[i].Reserved = 0;
		BootInfo->T_MemoryMap[i].PhysicalStart = MPTR->PhysicalStart;
		BootInfo->T_MemoryMap[i].NumberOfPages = MPTR->NumberOfPages;
		BootInfo->T_MemoryMapInfo.TotalMemorySize += MPTR->NumberOfPages * 4096;
		MPTR = (EFI_MEMORY_DESCRIPTOR*)((UINT8*)MPTR + DescriptorSize);
	};

	BootInfo->T_MemoryMapInfo.MapKey = MapKey;
	Last_MemoryMapAddress = MemoryMap;
	Last_ToolOSMemoryMapAddress = BootInfo->T_MemoryMap;

out:
	return Status;
}
