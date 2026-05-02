/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. 
 * Copyright (c) 2026, "뿌댕이"
 * Project: ToolOS
 * File: ToolOSBootLoader.c
 * Version : 0.1.1
 * * Description:
 * 
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
