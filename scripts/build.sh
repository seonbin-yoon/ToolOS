#!/bin/bash
set -e

# global settings
QEMU_ROOT=${HOME}/run-ovmf
QEMU_DISK_ROOT=${HOME}/run-ovmf/hda-contents
OS_SRC=${HOME}/src/ToolOS
EDK2_SRC=${HOME}/edk2_src/edk2
BUILD_THREADS=$(($(nproc) * 2 + 1))

# Kernel Build Options
ELF_NAME="ToolOS"

# BootLoader Build Options
BUILD_OPTIONS="DEBUG" # RELEASE / DEBUG
BUILD_TOOL_CHAIN="GCC" # GCC / MSVC
BUILD_ARCH="X64" # x64 / i386
LOADER_NAME="BootLoader"

# Qemu Options
QEMU_MEM="2G"
CORES=1

if [[ ${BUILD_THREADS} == 1 ]]; then
	echo "CPU의 코어 수를 알 수 없습니다. 수동으로 지정해주십시오."
	exit 1
fi

echo "=============================="
echo " q   : qemu 실행"
echo " kb  : 커널 빌드"
echo " b   : 부트로더 빌드"
echo " e   : 스크립트 종료"
echo "=============================="
read -p "> " CMD

case "${CMD}" in
    q)
        echo "[QEMU 실행]"
        cd $QEMU_ROOT
		qemu-system-x86_64 \
		-d int,cpu_reset,guest_errors -D latest.log \
		-enable-kvm \
		-machine q35 \
		-bios bios.bin \
		-cpu host \
		-smp cores=${CORES},threads=1 \
		-m ${QEMU_MEM} \
		-drive file=fat:rw:hda-contents,format=raw,if=none,id=bootdisk \
		-device ide-hd,drive=bootdisk,bootindex=0 \
		-vga std \
		-serial stdio \
		-monitor vc \
		-net none
		# -device qemu-xhci \
	    # -device usb-tablet \
        ;;
    kb)
		echo "[kernel.elf 빌드]"
		make
		RET_CODE=$?
		if [[ ${RET_CODE} != 0 ]]; then
            echo "커널 빌드중 에러가 발생했습니다. 반환 코드는 ${RET_CODE}입니다."
            exit ${RET_CODE}
        fi

		cp -v ${OS_SRC}/${ELF_NAME}.elf ${QEMU_DISK_ROOT}
		echo "성공적으로 작업을 완료했습니다."
		;;
    b)
        echo "[부트로더 빌드]"
		cp -rv ${OS_SRC}/boot/* ${EDK2_SRC}/MdeModulePkg/Application/ToolOS/

		python3 ${OS_SRC}/scripts/sources.py "${EDK2_SRC}/MdeModulePkg/Application/ToolOS/ToolOS.inf" "${OS_SRC}/boot"
		RET_CODE=$?
		if [[ ${RET_CODE} != 0 ]]; then
            echo "소스 파일 등록중 실행 중 에러가 발생했습니다. 반환 코드는 ${RET_CODE}입니다."
            exit ${RET_CODE}
        fi

		build -p MdeModulePkg/MdeModulePkg.dsc -m MdeModulePkg/Application/ToolOS/ToolOS.inf -a ${BUILD_ARCH} -t ${BUILD_TOOL_CHAIN} -b ${BUILD_OPTIONS} -n ${BUILD_THREADS}
        cp -v ${EDK2_SRC}/Build/MdeModule/${BUILD_OPTIONS}_${BUILD_TOOL_CHAIN}/X64/${LOADER_NAME}.efi ${QEMU_DISK_ROOT}/EFI/BOOT/BOOTX64.EFI
        echo "성공적으로 작업을 완료했습니다."
        ;;
    e)
        echo "스크립트 종료"
        ;;
    *)
        echo "알 수 없는 명령: ${CMD}"
        exit 1
        ;;
esac
