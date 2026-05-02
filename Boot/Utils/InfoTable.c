/*
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (c) 2026 Seonbin Yoon
 * Project: ToolOS
 * File: InfoTable.c
 * Description:
 *   Creates and returns a structure that the kernel can read.
 */

#include "TBL.h"
#define CPU_HALT __asm__ __volatile__ ("cli; hlt")

TOOLOS_MASTER_MAP InfoTable = {0};

TOOLOS_MASTER_MAP* Set_InfoTable(VOID) {
  CHAR8 TOOLOS_INFOTABLE_Signature[16] = {'T', 'O', 'O', 'L', 'O', 'S', ' ', 'B', 'O', 'O', 'T', ' ', 'I', 'N', 'F', 'O'};
  EFI_STATUS Status;
  UINT64 InfoTableSize = sizeof(TOOLOS_MASTER_MAP);

  Status = gBS->AllocatePool(
    EfiLoaderData,
    InfoTableSize,
    (VOID **)&InfoTable
  );

  if (EFI_ERROR(Status)) {
    Print(L"[ERROR] Error Code: %r | Position: AllocatePool -> For InfoTable", Status);
    CPU_HALT;
  }

  for (UINTN i = 0; i < 16; i++) {
    InfoTable.Signature[i] = TOOLOS_INFOTABLE_Signature[i];
  }

  return &InfoTable;
}