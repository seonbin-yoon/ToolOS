#include "TBL.h"
#define CPU_HALT __asm__ __volatile__ ("cli; hlt")

void* Last_MemoryMapAddress = NULL;
void* Last_ToolOSMemoryMapAddress = NULL;

VOID Get_EFI_MemoryMap(IN TOOLOS_MASTER_MAP* BootInfo) {
  CHAR8 TOOLOS_INFOTABLE_Signature[16] = {'T', 'O', 'O', 'L', 'O', 'S', ' ', 'B', 'O', 'O', 'T', ' ', 'I', 'N', 'F', 'O'};
  
  if (CompareMem(BootInfo->Signature, TOOLOS_INFOTABLE_Signature, 16) != 0) {
    Print(L"[ERROR] Not Correct Signature | Position : Check Signature (Get_EFI_MemoryMap)");
    CPU_HALT;
  }

  EFI_STATUS Status;

  UINTN MemoryMapSize = 0;
  EFI_MEMORY_DESCRIPTOR* MemoryMap = NULL;
  UINTN MapKey = 0;
  UINTN DescriptorSize = 0;
  UINT32 DescriptorVersion = 0;
  
  UINTN TOOLOS_MemoryMapSize;
  EFI_MEMORY_DESCRIPTOR* MPTR;

  if (Last_MemoryMapAddress || Last_ToolOSMemoryMapAddress) {
    Print(L"MemoryMap Returned! | %p & %p\r\n", Last_MemoryMapAddress, Last_ToolOSMemoryMapAddress);
    gBS->FreePool(Last_MemoryMapAddress);
    gBS->FreePool(Last_ToolOSMemoryMapAddress);
    Last_MemoryMapAddress = NULL;
    Last_ToolOSMemoryMapAddress = NULL;
  }

  Status = gBS->GetMemoryMap(
    &MemoryMapSize,
    NULL,
    &MapKey,
    &DescriptorSize,
    &DescriptorVersion
  );

  if (Status != EFI_BUFFER_TOO_SMALL) {
    Print(L"[ERROR] Error Code: %r | Position : GetMemoryMap [1/5]", Status);
    CPU_HALT;
  }

  MemoryMapSize += DescriptorSize * 3;
  BootInfo->T_MemoryMapInfo.MemoryMapNums = MemoryMapSize / DescriptorSize;
  TOOLOS_MemoryMapSize = sizeof(TOOLOS_MEMORY_MAP) * BootInfo->T_MemoryMapInfo.MemoryMapNums;

  Status = gBS->AllocatePool(
    EfiLoaderData,
    TOOLOS_MemoryMapSize,
    (VOID **)&BootInfo->T_MemoryMap
  );

  if (EFI_ERROR(Status)) {
    Print(L"[ERROR] Error Code: %r | Position : GetMemoryMap [2/5]", Status);
    CPU_HALT;
  }

  Status = gBS->AllocatePool(
    EfiLoaderData,
    MemoryMapSize,
    (VOID **)&MemoryMap
  );

  if (EFI_ERROR(Status)) {
    Print(L"[ERROR] Error Code: %r | Position : GetMemoryMap [3/5]", Status);
    CPU_HALT;
  }

  Status = gBS->GetMemoryMap(
    &MemoryMapSize,
    MemoryMap,
    &MapKey,
    &DescriptorSize,
    &DescriptorVersion
  );
  
  if (EFI_ERROR(Status)) {
    Print(L"[ERROR] Error Code: %r | Position : GetMemoryMap [4/5]", Status);
    CPU_HALT;
  }

  MPTR = MemoryMap;
  if (BootInfo->T_MemoryMapInfo.MemoryMapNums < MemoryMapSize / DescriptorSize) {
    Print(L"[ERROR] Untrusted OS-specific memory-map buffer | Position : GetMemoryMap [5/5]");
    CPU_HALT;
  }

  BootInfo->T_MemoryMapInfo.MemoryMapNums = MemoryMapSize / DescriptorSize;

  for (UINTN i = 0; i < BootInfo->T_MemoryMapInfo.MemoryMapNums; i++) {
    BootInfo->T_MemoryMap[i].Type = MPTR->Type;
    BootInfo->T_MemoryMap[i].Reserved = 0;
    BootInfo->T_MemoryMap[i].PhysicalStart = MPTR->PhysicalStart;
    BootInfo->T_MemoryMap[i].NumberOfPages = MPTR->NumberOfPages;
    BootInfo->T_MemoryMapInfo.TotalMemorySize += MPTR->NumberOfPages * 4096;
    MPTR = (EFI_MEMORY_DESCRIPTOR*)((UINT8*)MPTR + DescriptorSize);
  };

  BootInfo->T_MemoryMapInfo.MapKey = MapKey;
  Last_MemoryMapAddress = MemoryMap;
  Last_ToolOSMemoryMapAddress = BootInfo->T_MemoryMap;
}