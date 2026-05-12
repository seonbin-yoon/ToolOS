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

EFI_MEMORY_DESCRIPTOR *MemoryMap = NULL;
VOID *InfoMemoryMap = NULL;

EFI_STATUS GetMemoryInfo(IN TOOLOS_BOOTINFO_TABLE* BootInfo) {
	EFI_STATUS Status;

	UINTN MemoryMapSize = 0;
	UINTN MapKey = 0;
	UINTN DescriptorSize = 0;
	UINT32 DescriptorVersion = 0;

	UINT64 MemoryMapNums = 0;
	UINT64 ToolOSMMapSize = 0;
	EFI_MEMORY_DESCRIPTOR *MPTR = NULL;

	if (BootInfo == NULL || CompareMem(BootInfo->Signature, TOOLOS_INFOTABLE_Signature, 16) != 0) {
		Status = EFI_INVALID_PARAMETER;
		goto out;
	}

	if (MemoryMap) {
		gBS->FreePool(MemoryMap);
		MemoryMap = NULL;
	}

	if (InfoMemoryMap) {
		gBS->FreePool(InfoMemoryMap);
		InfoMemoryMap = NULL;
	}

	Status = gBS->GetMemoryMap(
		&MemoryMapSize,
		NULL,
		&MapKey,
		&DescriptorSize,
		&DescriptorVersion
	);
	if (Status != EFI_BUFFER_TOO_SMALL)
		goto out;

	MemoryMapNums = (MemoryMapSize / DescriptorSize) + 5;
	MemoryMapSize += DescriptorSize * 5;
	ToolOSMMapSize = sizeof(TOOLOS_MEMORY_MAP) * MemoryMapNums;

	Status = gBS->AllocatePool(
		EfiLoaderData,
		MemoryMapSize,
		(VOID **)&MemoryMap
	);
	if (EFI_ERROR(Status))
		goto out;

	Status = gBS->AllocatePool(
		EfiLoaderData,
		ToolOSMMapSize,
		(VOID **)&BootInfo->MemoryMap
	);
	if (EFI_ERROR(Status)) {
		gBS->FreePool(MemoryMap);
		MemoryMap = NULL;
		goto out;
	}

	InfoMemoryMap = BootInfo->MemoryMap;

	Status = gBS->GetMemoryMap(
		&MemoryMapSize,
		MemoryMap,
		&MapKey,
		&DescriptorSize,
		&DescriptorVersion
	);

	if (MemoryMapNums < MemoryMapSize / DescriptorSize) {
		Status = EFI_BUFFER_TOO_SMALL;
		goto out;
	}

	MemoryMapNums = MemoryMapSize / DescriptorSize;
	BootInfo->MemoryMapInfo.MemoryMapNums = MemoryMapNums;
	BootInfo->MemoryMapInfo.MapKey = MapKey;
	BootInfo->MemoryMapInfo.TotalMemorySize = 0;

	MPTR = MemoryMap;
	for (UINT64 i = 0; i < MemoryMapNums; i++) {
		BootInfo->MemoryMap[i].Type = MPTR->Type;
		BootInfo->MemoryMap[i].NumberOfPages = MPTR->NumberOfPages;
		BootInfo->MemoryMap[i].PhysicalStart = MPTR->PhysicalStart;
		BootInfo->MemoryMap[i].Attribute = MPTR->Attribute;
		BootInfo->MemoryMapInfo.TotalMemorySize += MPTR->NumberOfPages * 4096;
		MPTR = (EFI_MEMORY_DESCRIPTOR*)((UINT8*)MPTR + DescriptorSize);
	}

out:
	return Status;
}
