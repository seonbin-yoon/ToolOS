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

#define LOADIMAGE_GUID gEfiLoadedImageProtocolGuid
#define FileSystem_GUID gEfiSimpleFileSystemProtocolGuid
#define _1MB 1048576
#define CPU_HALT __asm__ __volatile__ ("cli; hlt")

EFI_FILE_PROTOCOL* KernelFile;
ELFHeader ELF_HEADER;
ELFProgramHeader* ELF_PMHEADER;
void* Total_ELF_PMHeader = NULL;

VOID Ready_KernelLoad(IN EFI_HANDLE ThisHandle, IN CHAR16* Name) {
  EFI_STATUS Status;
  EFI_LOADED_IMAGE* LoadedImage;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* FileSystem;
  EFI_FILE_PROTOCOL* RootDC;
  
  Status = gBS->HandleProtocol(
    ThisHandle,
    &LOADIMAGE_GUID,
    (VOID **)&LoadedImage
  );
  if (EFI_ERROR(Status)) {
    Print(L"[ERROR] Failed to retrieve device information. Error Code: %r\r\nSolution: Please check that the boot device is properly connected and that there is no physical damage. If the issue persists, recommend updating your UEFI firmware.\r\nIt is safe to shut down by pressing the power button.", Status);
    CPU_HALT;
  }

  Status = gBS->HandleProtocol(
    LoadedImage->DeviceHandle,
    &FileSystem_GUID,
    (VOID **)&FileSystem
  );
  if (EFI_ERROR(Status)) {
    Print(L"[ERROR] Unable to check file system information on the device. Error Code: %r\r\nSolution: Boot from the installation media and run the \"/fix filesystem\" command.\r\nIt is safe to shut down by pressing the power button.", Status);
    CPU_HALT;
  }

  Status = FileSystem->OpenVolume(
    FileSystem,
    &RootDC
  );
  if (EFI_ERROR(Status)) {
    Print(L"[ERROR] Failed to activate file system. Error Code: %r\r\nSolution: Boot from the installation media and run the \"/fix filesystem\" command.\r\nIt is safe to shut down by pressing the power button.", Status);
    CPU_HALT;
  }

  Status = RootDC->Open(
    RootDC,
    &KernelFile,
    Name,
    EFI_FILE_MODE_READ,
    0
  );

  if (EFI_ERROR(Status)) {
    Print(L"[ERROR] Cannot open kernel file. Error Code: %r\r\nSolution: Boot from the installation media and run the \"/check kernelfile\" command.\r\nIt is safe to shut down by pressing the power button.", Status);
    CPU_HALT;
  }
}

VOID KernelLoadAddress(IN TOOLOS_MASTER_MAP* BootInfo) {
  CHAR8 TOOLOS_INFOTABLE_Signature[16] = {'T', 'O', 'O', 'L', 'O', 'S', ' ', 'B', 'O', 'O', 'T', ' ', 'I', 'N', 'F', 'O'};
  CHAR8 ELF_Signature[4] = {0x7F, 'E', 'L', 'F'};
  
  if (CompareMem(BootInfo->Signature, TOOLOS_INFOTABLE_Signature, 16) != 0) {
    Print(L"[ERROR] Not Correct Signature | Position : Check Signature (Get_EFI_MemoryMap)");
    CPU_HALT;
  }

  Print(L"Pass. [1]\r\n");
  
  EFI_STATUS Status;
  UINTN KernelNeedMemory = 0;
  UINTN KernelNeedPages = 0;
  BOOLEAN KernelLoadSafe = FALSE;

  UINTN ELF_Header_Size = 0;
  UINTN ELF_PMHeader_Size = 0;
  UINTN Total_ELF_PMHeader_Size = 0;

  ELF_Header_Size = sizeof(ELFHeader);
  ELF_PMHeader_Size = sizeof(ELFProgramHeader);

  Print(L"Pass. [2]\r\n KernelFile : 0x%p\r\n", KernelFile);

  Status = KernelFile->Read(
    KernelFile,
    &ELF_Header_Size,
    (VOID *)&ELF_HEADER
  );

  if (EFI_ERROR(Status)) {
    Print(L"[ERROR] Failed Read Kernel File.");
    CPU_HALT;
  }

  Print(L"Pass. [3]\r\n");

  if (CompareMem(ELF_HEADER.e_ident, ELF_Signature, 4) != 0 ) {
    Print(L"[ERROR] Kernel File not ELF Format.");
    CPU_HALT;
  }

  if (ELF_HEADER.e_ident[4] != 2) {
    Print(L"[ERROR] Kernel File not 64bit.");
    CPU_HALT;
  }

  if (ELF_HEADER.e_ident[5] != 1) {
    Print(L"[ERROR] Kernel File not Little Endian.");
    CPU_HALT;
  }

  Print(L"Pass. [4]\r\n");

  Status = KernelFile->SetPosition(
    KernelFile,
    ELF_HEADER.e_phoff
  );

  if (EFI_ERROR(Status)) {
    Print(L"[ERROR] Failed Read Kernel File.");
    CPU_HALT;
  }

  Print(L"Pass. [5]\r\n");

  Total_ELF_PMHeader_Size = ELF_PMHeader_Size * ELF_HEADER.e_phnum;

  Status = gBS->AllocatePool(
    EfiLoaderData,
    Total_ELF_PMHeader_Size,
    (VOID **)&Total_ELF_PMHeader
  );

  if (EFI_ERROR(Status)) {
    Print(L"[ERROR] Failed Read KernelFile.");
    CPU_HALT;
  }

  Status = KernelFile->Read(
    KernelFile,
    &Total_ELF_PMHeader_Size,
    (VOID *)Total_ELF_PMHeader
  );

  if (EFI_ERROR(Status)) {
    Print(L"[ERROR] Failed Read KernelFile.");
    CPU_HALT;
  }

  ELF_PMHEADER = Total_ELF_PMHeader;

  for (UINTN i = 0; i < ELF_HEADER.e_phnum; i++) {
    if (ELF_PMHEADER[i].p_type == 1) {
      KernelNeedMemory += ELF_PMHEADER[i].p_memsz;
    }
  }

  Print(L"Pass. [6]\r\n KernelNeedMemory : %dbyte.", KernelNeedMemory);
  CPU_HALT;

  for (UINTN i = 0; i < BootInfo->T_MemoryMapInfo.MemoryMapNums; i++) {
    if (BootInfo->T_MemoryMap[i].Type == EfiConventionalMemory && _1MB <= BootInfo->T_MemoryMap[i].PhysicalStart && KernelNeedMemory < BootInfo->T_MemoryMap[i].NumberOfPages * 4096) {
      BootInfo->KernelStartAddress = BootInfo->T_MemoryMap[i].PhysicalStart;
      KernelLoadSafe = TRUE;
      break;
    }
  }

  if (!KernelLoadSafe) {
    Print(L"[ERROR] Kernel Load are Not Safe.");
    CPU_HALT;
  }

  if (KernelNeedMemory < 4096) {
    KernelNeedPages = 1;
  } else { 
    KernelNeedPages = (KernelNeedMemory / 4096) + 1;
  }

  Status = gBS->AllocatePages(
    AllocateAddress,
    EfiLoaderData,
    KernelNeedPages,
    (VOID *)BootInfo->KernelStartAddress
  );

  if (EFI_ERROR(Status)) {
    Print(L"[ERROR] Can't AllocatePages for Kernel Image. | Position : KernelLoadAddress");
    CPU_HALT;
  }
}

VOID KernelLoad(VOID) {

}

VOID End_KernelLoad(VOID) {
  if (KernelFile != NULL) {
    KernelFile->Close(
      KernelFile
    );
    KernelFile = NULL;
  }
}
