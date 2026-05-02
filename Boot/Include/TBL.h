#ifndef TBL_H
#define TBL_H

#include <Uefi.h>
#include <Uefi/UefiBaseType.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Guid/Acpi.h>
#include <Guid/FileInfo.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/LoadedImage.h>
#include "InfoTable.h"
#include "ELF.h"

// typedef (*GoToKernel)(TOOLOS_MASTER_MAP* BootInfo);

TOOLOS_MASTER_MAP*
(Set_InfoTable)(
  VOID
);

VOID
(Get_EFI_MemoryMap)(
  IN TOOLOS_MASTER_MAP* BootInfo
);

VOID
(Get_ACPI_Table)(
  IN TOOLOS_MASTER_MAP* BootInfo
);

VOID
(Get_GOP_Info)(
  IN TOOLOS_MASTER_MAP* BootInfo
);

VOID
(Ready_KernelLoad)(
  IN EFI_HANDLE ThisHandle,
  IN CHAR16* Name
);


VOID
(KernelLoadAddress)(
  IN TOOLOS_MASTER_MAP* BootInfo
);

VOID
(KernelLoad)(
  VOID
);

VOID
(End_KernelLoad)(
  VOID
);

#endif