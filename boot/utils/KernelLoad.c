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
#define ELF_MAGIC_NUM 0x464C457F
#define PT_LOAD 1

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

EFI_STATUS ValidationELFHeader(IN EFI_FILE_PROTOCOL *File, IN BOOLEAN IsBigEndian) {
	EFI_STATUS Status;
	ELFHeader EhdrReader;
	UINTN EhdrSize = sizeof(ELFHeader);
	UINT64 EphdrSize = sizeof(ELFProgramHeader);

	if (File == NULL) {
		Status = EFI_INVALID_PARAMETER;
		goto out;
	}

	Status = File->Read(
		File,
		&EhdrSize,
		&EhdrReader
	);
	if (EFI_ERROR(Status))
		goto rollback_out;

	if (*(UINT32 *)EhdrReader.e_ident != ELF_MAGIC_NUM) {
		Status = EFI_UNSUPPORTED;
		goto rollback_out;
	}

	if (EhdrReader.e_ident[4] != 2 || EhdrReader.e_ident[5] != (IsBigEndian + 1)) {
		Status = EFI_UNSUPPORTED;
		goto rollback_out;
	}

	if (EhdrReader.e_phentsize != EphdrSize) {
		Status = EFI_UNSUPPORTED;
		goto rollback_out;
	}

rollback_out:
	File->SetPosition(File, 0);
out:
	return Status;
}

EFI_STATUS GetKernelFileSize(EFI_FILE_PROTOCOL *File, UINT64 *SizeBuffer) {
	EFI_STATUS Status;
	
	ELFHeader EhdrReader;
	ELFProgramHeader *EphdrReader = NULL;

	UINTN EhdrSize = sizeof(ELFHeader);
	VOID *TotalPhdr = NULL;
	UINTN TotalPhdrSize = 0;

	UINT64 MinSegAddr = -1;
	UINT64 MaxSegAddr = 0;

	if (File == NULL || SizeBuffer == NULL) {
		Status = EFI_INVALID_PARAMETER;
		goto out;
	}

	Status = File->Read(
		File,
		&EhdrSize,
		&EhdrReader
	);
	if (EFI_ERROR(Status))
		goto rollback_out;

	TotalPhdrSize = EhdrReader.e_phnum * EhdrReader.e_phentsize;
	Status = gBS->AllocatePool(
		EfiLoaderData,
		TotalPhdrSize,
		&TotalPhdr
	);
	if (EFI_ERROR(Status))
		goto rollback_out;

	Status = File->SetPosition(
		File,
		EhdrReader.e_phoff
	);
	if (EFI_ERROR(Status))
		goto free_phdrbuffer_out;

	Status = File->Read(
		File,
		&TotalPhdrSize,
		TotalPhdr
	);
	if (EFI_ERROR(Status))
		goto free_phdrbuffer_out;

	EphdrReader = TotalPhdr;
	for (UINT64 i = 0; i < EhdrReader.e_phnum; i++) {
		if (EphdrReader[i].p_type == PT_LOAD) {
			if (EphdrReader[i].p_vaddr < MinSegAddr)
				MinSegAddr = EphdrReader[i].p_vaddr;
			if ((EphdrReader[i].p_vaddr + EphdrReader[i].p_memsz) > MaxSegAddr)
				MaxSegAddr = EphdrReader[i].p_vaddr + EphdrReader[i].p_memsz;
		}
	}

	if (MaxSegAddr < MinSegAddr) {
		Status = EFI_LOAD_ERROR;
		goto free_phdrbuffer_out;
	}

	*SizeBuffer = MaxSegAddr - MinSegAddr;

free_phdrbuffer_out:
	gBS->FreePool(TotalPhdr);
rollback_out:
	File->SetPosition(File, 0);
out:
	return Status;
}

EFI_STATUS LoadKernelFile(TOOLOS_MASTER_MAP *BootInfo, EFI_FILE_PROTOCOL *File, EFI_PHYSICAL_ADDRESS LoadAddress, UINT64 MemSize) {
	EFI_STATUS Status;
	UINT64 NeedPages = 0;
	UINT64 NeedSize = 0;
	EFI_PHYSICAL_ADDRESS KernelAddress = LoadAddress;

	UINTN EhdrSize = sizeof(ELFHeader);
	ELFHeader EhdrReader;
	ELFProgramHeader *EphdrReader = NULL;
	VOID *TotalPhdr = NULL;
	UINTN TotalPhdrSize = 0;

	UINT64 MinSegAddr = MAX_UINT64;
	UINT64 ReadOffset = 0;

	if (File == NULL || !LoadAddress || !MemSize) {
		Status = EFI_INVALID_PARAMETER;
		goto out;
	}
	
	NeedPages = EFI_SIZE_TO_PAGES(MemSize);
	NeedSize = NeedPages * EFI_PAGE_SIZE;

	Status = gBS->AllocatePages(
		AllocateAddress,
		EfiLoaderData,
		NeedPages,	
		&KernelAddress
	);
	if (EFI_ERROR(Status))
		goto out;

	gBS->SetMem(
		(VOID *)KernelAddress,
		NeedSize,
		0
	);

	Status = File->Read(
		File,
		&EhdrSize,
		&EhdrReader
	);
	if (EFI_ERROR(Status))
		goto out;

	TotalPhdrSize = EhdrReader.e_phentsize * EhdrReader.e_phnum;
	Status = gBS->AllocatePool(
		EfiLoaderData,
		TotalPhdrSize,
		&TotalPhdr
	);
	if (EFI_ERROR(Status))
		goto rollback_out;

	Status = File->SetPosition(
		File,
		EhdrReader.e_phoff
	);
	if (EFI_ERROR(Status))
		goto free_phdrbuffer_out;

	Status = File->Read(
		File,
		&TotalPhdrSize,
		TotalPhdr
	);
	if (EFI_ERROR(Status))
		goto free_phdrbuffer_out;

	EphdrReader = (ELFProgramHeader *)TotalPhdr;
	for (UINT64 i = 0; i < EhdrReader.e_phnum; i++) {
		if (EphdrReader[i].p_type == PT_LOAD) {
			if (EphdrReader[i].p_vaddr < MinSegAddr)
				MinSegAddr = EphdrReader[i].p_vaddr;
		}
	}

	for (UINT64 i = 0; i < EhdrReader.e_phnum; i++) {
		if (EphdrReader[i].p_type == PT_LOAD) {
			Status = File->SetPosition(
				File,
				EphdrReader[i].p_offset
			);
			if (EFI_ERROR(Status))
				goto free_phdrbuffer_out;

			ReadOffset = KernelAddress + EphdrReader[i].p_vaddr - MinSegAddr;

			Status = File->Read(
				File,
				&EphdrReader[i].p_filesz,
				(VOID *)ReadOffset
			);
			if (EFI_ERROR(Status))
				goto free_phdrbuffer_out;
		}
	}

	BootInfo->KernelStartAddress = KernelAddress + (EhdrReader.e_entry - MinSegAddr);

free_phdrbuffer_out:
	gBS->FreePool(TotalPhdr);
rollback_out:
	File->SetPosition(File, 0);
out:
	return Status;
}

EFI_STATUS CloseKernelFile(EFI_FILE_PROTOCOL *File) {
	EFI_STATUS Status;

	Status = File->Close(File);
	return Status;
} 
