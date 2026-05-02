#include "TBL.h"

#define ACPI_GUID gEfiAcpi20TableGuid
#define CPU_HALT __asm__ __volatile__ ("cli; hlt")

VOID Get_ACPI_Table(IN TOOLOS_MASTER_MAP* BootInfo) {
  CHAR8 TOOLOS_INFOTABLE_Signature[16] = {'T', 'O', 'O', 'L', 'O', 'S', ' ', 'B', 'O', 'O', 'T', ' ', 'I', 'N', 'F', 'O'};
  
  if (CompareMem(BootInfo->Signature, TOOLOS_INFOTABLE_Signature, 16) != 0) {
    Print(L"[ERROR] Not Correct Signature | Position : Check Signature (Get_ACPI_Table)");
    CPU_HALT;
  }
  
  CHAR8 ACPI_TABLE_Signature[8] = {'R', 'S', 'D', ' ', 'P', 'T', 'R', ' '};
  
  for (UINTN i = 0; i < gST->NumberOfTableEntries; i++) {
    if(CompareGuid(&gST->ConfigurationTable[i].VendorGuid, &ACPI_GUID)) {
      if (CompareMem(gST->ConfigurationTable[i].VendorTable, &ACPI_TABLE_Signature, 8) == 0) {
        BootInfo->T_ACPI_Table = (TOOLOS_ACPI_XSDT_TABLE*)gST->ConfigurationTable[i].VendorTable;
        return;
      } else {
        Print(L"[ERROR] Can't Found ACPI Table. | Position: Get_ACPI_Table");
        CPU_HALT;
      }
    }
  }
}