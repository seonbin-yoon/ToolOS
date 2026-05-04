#!/bin/bash
set -e

SRC=$HOME/ToolOS
QEMUROOT=$HOME/run-ovmf/hda-contents

echo "=============================="
echo " q   : qemu 실행"
echo " kb  : TOSKernel.elf 빌드"
echo " b   : 부트로더 빌드"
echo " e   : 스크립트 종료"
echo "=============================="
read -p "입력: " CMD

case "$CMD" in
    q)
        echo "[QEMU 실행]"
        cd ~/run-ovmf
		qemu-system-x86_64 \
		-d int,cpu_reset,guest_errors -D qemu.log \
	    -accel kvm \
	    -machine q35 \
	    -bios ./bios.bin \
	    -cpu host \
	    -smp cores=8,threads=1 \
	    -m 2048 \
	    -drive file=fat:rw:hda-contents,format=raw,if=none,id=bootdisk \
	    -device ide-hd,drive=bootdisk,bootindex=0 \
	    -vga std \
	    -device qemu-xhci \
	    -device usb-tablet \
	    -serial stdio \
	    -monitor vc \
	    -net none
        ;;

    kb)
		echo "[kernel.elf 빌드]"
		mkdir -p $SRC/build
		nasm -f elf64 $SRC/kernel/init.S -o $SRC/build/init.o
		gcc -I $SRC/Include -c $SRC/Kernel/Kernel.c -o $SRC/Build/kernel.o \
		   -ffreestanding -fno-stack-protector -mno-red-zone -fPIC -m64
		ld -m elf_x86_64 -L $SRC/build -o $SRC/TOSKernel.elf $SRC/build/init.o $SRC/build/kernel.o -T $SRC/kernel/kernel.ld -static -nostdlib
		cp $SRC/TOSKernel.elf $QEMUROOT/TOSKernel.elf
		echo "성공적으로 작업을 완료했습니다."
		;;
    b)
        echo "[부트로더 빌드]"
		cp -rv $SRC/boot/* ~/src/edk2/MdeModulePkg/Application/ToolOS/
		python $SRC/scripts/sources.py
		build -p MdeModulePkg/MdeModulePkg.dsc -m MdeModulePkg/Application/ToolOS/ToolOS.inf -a X64 -t GCC -b RELEASE -n 17
        cp ~/src/edk2/Build/MdeModule/RELEASE_GCC/X64/BootLoader.efi \
           ~/run-ovmf/hda-contents/EFI/BOOT/BOOTX64.EFI
        echo "성공적으로 작업을 완료했습니다."
        ;;

    e)
        echo "스크립트 종료"
		exit 1
        ;;

    *)
        echo "알 수 없는 명령: $CMD"
        exit 1
        ;;
esac
