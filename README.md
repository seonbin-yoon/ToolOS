# ToolOS Source Tree

**ToolOS** is an operating system currently under development **that prioritizes security and user privacy above all else**.

## Special Notes
* ToolOS is currently being developed exclusively for **ARM 64-bit CPUs**.
* If you need to compile the OS, please refer to this document. This project uses clang as its compiler and assembler, lld as its linker, and adheres to the C23 standard.
* **The `boot` folder is related to the UEFI (EDK2) boot loader.** To compile the boot loader, run `git clone https://github.com/seonbin-yoon/edk2-setup`, then run the `installer` file located in the ‘bin/’ directory of that folder to set up the compilation environment.

## Core technologies

### 1. KCP (Kernel Camera Protection)
* KCP is a security architecture in which the kernel isolates the camera data bus, allowing access only to authorized processes. This architecture was designed to eliminate threats such as webcam hijacking, unauthorized leakage of facial information, and physical privacy violations, and we hope it will help users who are particularly concerned about the leakage of their facial images, such as VTubers.

### 2. SFS (Secure File System)
* SFS is designed to support dynamic partition resizing, selective-area encryption, and file system updates while the system is booted. By strategically distributing data across specific sectors on the node, it prevents the loss of all data due to damage to a single section.
* Adopts Drive Letter Assignment with a Forward-Slash (/) path separator (e.g., C:/ToolOS/).

### 3. AAPI (Authentication API)
* One of the major obstacles to adopting open-source operating systems is the lack of standardization, as seen in the variety of Linux distributions. This fragmentation makes it difficult for developers to provide consistent support. ToolOS addresses this issue by providing an authentication API that verifies the integrity of the system (genuine vs. modified or ported versions). This allows developers to focus their support on verified environments, ensuring a stable and secure experience for both creators and users.

## Project Roadmap
* **Project Started:**  11, 2026
* **Developer Age at Launch:** 14 Years Old

## Contributions
* I welcome feedback. If you’d like to provide feedback, please contact me at seonbin.yoon0@gmail.com.