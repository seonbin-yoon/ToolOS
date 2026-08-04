# ToolOS Source Tree

**ToolOS** is an operating system currently under development **that prioritizes security and user privacy above all else**.

## Special Notes
* ToolOS is currently being developed exclusively for **64-bit CPUs**.
* Development is currently based on the intel® i7-9700 CPU.
* If you need to compile the OS, you should refer to this. This project uses gcc as the compiler, the C23 standard, and nasm as the assembler.
* **The `boot` folder is associated with the UEFI (EDK2) bootloader.** If you want to compile the bootloader, run `git clone https://github.com/seonbin-yoon/edk2-setup-release` and then run the `installer` file inside the folder to set up the compilation environment.

## Core technologies

### 1. KCP (Kernel Camera Protection)
* KCP is a security architecture in which the kernel isolates the camera data bus, allowing access only to authorized processes. This architecture was designed to eliminate threats such as webcam hijacking, unauthorized leakage of facial information, and physical privacy violations, and we hope it will help users who are particularly concerned about the leakage of their facial images, such as VTubers.

### 2. SFS (Secure File System)
* SFS is designed to support dynamic partition resizing, selective-area encryption, and file system updates while the system is booted. By strategically distributing data across specific sectors on the node, it prevents the loss of all data due to damage to a single section.
* Adopts Drive Letter Assignment with a Forward-Slash (/) path separator (e.g., C:/ToolOS/).

### 3. AAPI (Authentication API)
* One of the major obstacles to adopting open-source operating systems is the lack of standardization, as seen in the variety of Linux distributions. This fragmentation makes it difficult for developers to provide consistent support. ToolOS addresses this issue by providing an authentication API that verifies the integrity of the system (genuine vs. modified or ported versions). This allows developers to focus their support on verified environments, ensuring a stable and secure experience for both creators and users.

## Project Roadmap
* **Project Started:** January 9, 2026
* **Developer Age at Launch:** 14 Years Old

## Contributions
* I welcome feedback. If you’d like to provide feedback, please contact me at seonbin.yoon0@gmail.com.