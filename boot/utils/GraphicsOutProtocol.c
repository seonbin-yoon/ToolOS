/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (c) 2026 Seonbin Yoon
 * Project: ToolOS
 * File: GraphicsOutProtocol.c
 * Description:
 *   Retrieving GOP information.
 */

#include "TBL.h"

EFI_STATUS GetGOPInfo(IN TOOLOS_BOOTINFO_TABLE* BootInfo) {
	EFI_STATUS Status;
	EFI_GRAPHICS_OUTPUT_PROTOCOL* GOP = NULL;
	
	if (BootInfo == NULL || CompareMem(BootInfo->Signature, TOOLOS_INFOTABLE_Signature, 16) != 0) {
		Status = EFI_INVALID_PARAMETER;
		goto out;
	}

	Status = gBS->LocateProtocol(
		&gEfiGraphicsOutputProtocolGuid,
		NULL,
		(VOID **)&GOP
	);
	if (EFI_ERROR(Status))
		goto out;

	BootInfo->GraphicsMap.FrameBufferBase                = GOP->Mode->FrameBufferBase;
	BootInfo->GraphicsMap.FrameBufferSize                = GOP->Mode->FrameBufferSize;
	BootInfo->GraphicsMap.HorizontalResolution           = GOP->Mode->Info->HorizontalResolution;
	BootInfo->GraphicsMap.VerticalResolution             = GOP->Mode->Info->VerticalResolution;
	BootInfo->GraphicsMap.PixelInformation.BlueMask      = GOP->Mode->Info->PixelInformation.BlueMask;
	BootInfo->GraphicsMap.PixelInformation.GreenMask     = GOP->Mode->Info->PixelInformation.GreenMask;
	BootInfo->GraphicsMap.PixelInformation.RedMask       = GOP->Mode->Info->PixelInformation.RedMask;
	BootInfo->GraphicsMap.PixelInformation.ReservedMask  = GOP->Mode->Info->PixelInformation.ReservedMask;
	BootInfo->GraphicsMap.PixelsPerScanLine              = GOP->Mode->Info->PixelsPerScanLine;
	BootInfo->GraphicsMap.Version                        = GOP->Mode->Info->Version;

out:
	return Status;
}
