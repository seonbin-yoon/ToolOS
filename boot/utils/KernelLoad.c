/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (c) 2026 Seonbin Yoon
 * Project: ToolOS
 * File: KernelLoad.c
 * Description:
 *   It is responsible for kernel loading, addressing, and load preparation.
 */

#include "TBL.h"

EFI_STATUS OpenKernelFile(IN EFI_HANDLE BootLoaderHandle, IN CHAR16 *FileName, IN OUT EFI_FILE_PROTOCOL **File) {
    EFI_STATUS Status;
    EFI_LOADED_IMAGE *BootLoaderInfo = NULL;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem = NULL;
    EFI_FILE_PROTOCOL *Root = NULL;

    if (BootLoaderHandle == NULL || FileName == NULL || File == NULL) {
        Status = EFI_INVALID_PARAMETER;
        goto out;
    }

    *File = NULL;

    Status = gBS->HandleProtocol(
        BootLoaderHandle,
        &gEfiLoadedImageProtocolGuid,
        (VOID **)&BootLoaderInfo
    );
    if (EFI_ERROR(Status))
        goto out;

    Status = gBS->HandleProtocol(
        BootLoaderInfo->DeviceHandle,
        &gEfiSimpleFileSystemProtocolGuid,
        (VOID **)&FileSystem
    );
    if (EFI_ERROR(Status))
        goto out;

    Status = FileSystem->OpenVolume(
        FileSystem,
        &Root
    );
    if (EFI_ERROR(Status))
        goto out_close_root;

    Status = Root->Open(
        Root,
        File,
        FileName,
        EFI_FILE_MODE_READ,
        0
    );

out_close_root:
    if (Root)
        Root->Close(Root);
out:
    return Status;
}
