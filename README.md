# ToolOS

**ToolOS** is a hobby operating system built from scratch to explore OS internals and architecture principles in depth.

## Specifications & Requirements

* **Target Architecture:** ARM 64-bit (`AArch64`)
* **Bootloader Firmware:** UEFI (EDK2)
* **Language & Standard:** C (adhering to the **C23** standard, excluding the bootloader)
* **Toolchain:** Clang (Compiler/Assembler), LLD (Linker)
* **Emulator:** QEMU (`qemu-system-aarch64`)

## How to Build

### 1. Set Up the Environment
If you haven't configured the build environment yet, clone and run the automated setup utility:
```bash
git clone https://github.com/seonbin-yoon/auto-setup-tde
```
Refer to its `README.md` to run `installer.bin` (located in `bin/installer.dist`) and configure the complete environment required to build all components of ToolOS.

### 2. Clone the Repository
```bash
git clone https://github.com/seonbin-yoon/ToolOS
cd ToolOS
```

### 3. Configure the Build
Copy the sample configuration file to create your local `global.cfg`:
```bash
cp global.cfg.example global.cfg
```
**`global.cfg` serves as the single source of truth for build automation. Inspect the comments inside and adjust the configuration to match your environment.**

### 4. Set Up EDK II Build Settings
Copy the example configuration files in the `boot/edk2_settings` directory to create `ToolOS.dsc` and `ToolOS.inf`:
```bash
cp boot/edk2_settings/ToolOS.dsc.example boot/edk2_settings/ToolOS.dsc
cp boot/edk2_settings/ToolOS.inf.example boot/edk2_settings/ToolOS.inf
```
If you are using `build.sh`, you will not need to make any manual changes to the contents of these files.

### 5. Build ToolOS
Run the build script with the `b` (bootloader) and `kb` (kernel build) arguments:
```bash
./scripts/build.sh b kb
```
This builds the bootloader and kernel sequentially. The built binaries are automatically placed into the hda-contents directory under your configured QEMU_WORKSPACE (defined in global.cfg), structuring the actual disk layout for QEMU.

### 6. Run with QEMU (Debug Mode)
Once `ToolOS.img` is generated, launch the emulator:
```bash
./scripts/build.sh q
```
* QEMU starts in debug mode (`-s -S`). The CPU will halt on boot and wait for a GDB client to connect on `localhost:1234`.

## License

ToolOS and all of its components are licensed under the **GNU General Public License v2.0**. 

This license applies exclusively to files that include an explicit copyright header. Any file without a copyright header is NOT covered by the GPL-2.0. For full license terms, please refer to the `LICENSE` file.

## Feedback & Contributions
* * Feedback, suggestions, and questions are always welcome. Please contact me at <seonbin.yoon0@gmail.com>.