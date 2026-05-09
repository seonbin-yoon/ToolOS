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

EFI_STATUS Get_GOP_Info(IN TOOLOS_MASTER_MAP* BootInfo) {
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

	BootInfo->T_GraphicsMap.FrameBufferBase                = GOP->Mode->FrameBufferBase;
	BootInfo->T_GraphicsMap.FrameBufferSize                = GOP->Mode->FrameBufferSize;
	BootInfo->T_GraphicsMap.HorizontalResolution           = GOP->Mode->Info->HorizontalResolution;
	BootInfo->T_GraphicsMap.VerticalResolution             = GOP->Mode->Info->VerticalResolution;
	BootInfo->T_GraphicsMap.PixelInformation.BlueMask      = GOP->Mode->Info->PixelInformation.BlueMask;
	BootInfo->T_GraphicsMap.PixelInformation.GreenMask     = GOP->Mode->Info->PixelInformation.GreenMask;
	BootInfo->T_GraphicsMap.PixelInformation.RedMask       = GOP->Mode->Info->PixelInformation.RedMask;
	BootInfo->T_GraphicsMap.PixelInformation.ReservedMask  = GOP->Mode->Info->PixelInformation.ReservedMask;
	BootInfo->T_GraphicsMap.PixelsPerScanLine              = GOP->Mode->Info->PixelsPerScanLine;
	BootInfo->T_GraphicsMap.Version                        = GOP->Mode->Info->Version;

out:
	return Status;
}
