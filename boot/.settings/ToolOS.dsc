[Defines]
    PLATFORM_NAME           = ToolOS
    PLATFORM_GUID           = C10BC2F4-049B-4A4A-A2BE-06F94278EBD8
    PLATFORM_VERSION        = 0.0.1
    DSC_SPECIFICATION       = 0x0001001C
    SUPPORTED_ARCHITECTURES = AARCH64
    BUILD_TARGETS           = DEBUG
    SKUID_IDENTIFIER        = DEFAULT

[LibraryClasses]
    UefiApplicationEntryPoint|MdePkg/Library/UefiApplicationEntryPoint/UefiApplicationEntryPoint.inf
    UefiDriverEntryPoint|MdePkg/Library/UefiDriverEntryPoint/UefiDriverEntryPoint.inf

    UefiBootServicesTableLib|MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
    UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf

    UefiLib|MdePkg/Library/UefiLib/UefiLib.inf
    BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
    BaseMemoryLib|MdePkg/Library/BaseMemoryLib/BaseMemoryLib.inf
    MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf

    PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
    DebugLib|MdePkg/Library/UefiDebugLibConOut/UefiDebugLibConOut.inf
    DebugPrintErrorLevelLib|MdePkg/Library/BaseDebugPrintErrorLevelLib/BaseDebugPrintErrorLevelLib.inf

    PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
    DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
    StackCheckLib|MdePkg/Library/StackCheckLibNull/StackCheckLibNull.inf

[Components]
    .settings/ToolOS.inf