#include "TBL.h"

#define GOP_GUID gEfiGraphicsOutputProtocolGuid
#define CPU_HALT __asm__ __volatile__ ("cli; hlt")


VOID Get_GOP_Info(IN TOOLOS_MASTER_MAP* BootInfo) {
  CHAR8 TOOLOS_INFOTABLE_Signature[16] = {'T', 'O', 'O', 'L', 'O', 'S', ' ', 'B', 'O', 'O', 'T', ' ', 'I', 'N', 'F', 'O'};
  
  if (CompareMem(BootInfo->Signature, TOOLOS_INFOTABLE_Signature, 16) != 0) {
    Print(L"[ERROR] Not Correct Signature | Position : Check Signature (Get_EFI_MemoryMap)");
    CPU_HALT;
  }
  
  EFI_STATUS Status;
  EFI_GRAPHICS_OUTPUT_PROTOCOL* GOP = NULL;
  
  Status = gBS->LocateProtocol(
    &GOP_GUID,
    NULL,
    (VOID **)&GOP
  );

  if (EFI_ERROR(Status)) {
    Print(L"[ERROR] Can't Found GOP Address. | Position: Get_GOP_Info");
    CPU_HALT;
  }

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
}