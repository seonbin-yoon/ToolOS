/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (c) 2026 Seonbin Yoon
 * Project: ToolOS
 * File: BootLoader.c
 * Description: 
 *   This is the main bootloader for ToolOS
 */

#include "TBL.h"

#define CPU_HALT __asm__ __volatile__ ("cli; hlt")

EFI_STATUS EFIAPI Main(EFI_HANDLE ThisHandle, EFI_SYSTEM_TABLE* SystemTable) {
  SystemTable->ConOut->ClearScreen(SystemTable->ConOut);

  // EFI_STATUS Status;
  TOOLOS_MASTER_MAP* BootInfo;
  // GoToKernel JumpToKernel;

  BootInfo = Set_InfoTable();

  Get_ACPI_Table(BootInfo);

  Get_GOP_Info(BootInfo);

  Ready_KernelLoad(ThisHandle, L"TOSKernel.elf");
  Get_Kernel_Size();
  End_KernelLoad();

  CPU_HALT;

  return EFI_SUCCESS;
}
