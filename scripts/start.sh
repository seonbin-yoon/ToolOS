#!/usr/bin/env bash

function error_check() {
    if [[ $1 != 0 ]]; then
        echo "An error occurred while executing the task. The return code is $1."
        exit $1
    fi
}

function run_qemu() {
    echo "[Running QEMU]"
    echo "CPU: ${QEMU_CPU_CORE} Core"
    echo "MEM: ${QEMU_MEM}"

    cd ${QEMU_ROOT}
    qemu-system-aarch64 \
    -bios /usr/share/qemu-efi-aarch64/QEMU_EFI.fd \
    -machine virt \
    -cpu cortex-a710 \
    -smp cores=${QEMU_CPU_CORE},threads=1 \
    -m ${QEMU_MEM} \
    -drive file=fat:rw:hda-contents,format=raw,if=none,id=hd0 \
    -device virtio-gpu-pci \
    -device virtio-blk-pci,drive=hd0,bootindex=0 \
    -serial stdio \
    -net none \
    -s -S
}

cd $(cd $(dirname $BASH_SOURCE) && pwd)/../
CONF_FILE="global.cfg"

if [[ ! -f ${CONF_FILE} ]]; then
    echo "ERR: '${CONF_FILE}' file does not exist."
    exit 1
fi

eval "$(sed -e '/^\[/d' -e 's/ *#.*//' -e '/^$/d' "${CONF_FILE}")"

if [[ ! "${BUILD_THREADS}" =~ ^[0-9]+$ ]]; then
    BUILD_THREADS=$(($(nproc) * 2 + 1))
fi

echo
echo " q   : Running QEMU (with gdb-multiarch)"
echo " b   : BootLoader Build"
echo " kb  : Kernel Build"
echo

read -p "> " CMD

clear
case "${CMD}" in
    q)
        run_qemu 
        error_check $?
        ;;
    b)
        echo "[BootLoader Build]"
        export PACKAGES_PATH="${EDK2_SRC}:${OS_SRC}/boot"
		python3 ${OS_SRC}/scripts/sources.py ${OS_SRC}/boot/.settings/ToolOS.inf ${OS_SRC}/boot
        error_check $?
		build -p .settings/ToolOS.dsc -m .settings/ToolOS.inf -a AARCH64 -t CLANGPDB -b ${BUILD_OPTIONS} -n ${BUILD_THREADS}
        error_check $?
        cp -v ${EDK2_SRC}/Build/ToolOS/${BUILD_OPTIONS}_CLANGPDB/AARCH64/BootLoader.efi ${QEMU_DISK_ROOT}/EFI/BOOT/BOOTAA64.EFI
        error_check $?
        ;;
    kb)
		echo "[Kernel Build (ToolOS.elf)]"
        make
        error_check $?
		cp -v ${OS_SRC}/ToolOS.elf ${QEMU_DISK_ROOT}
        error_check $?
		;;
    *)
        echo "Invalid command: ${CMD}"
        exit 1
        ;;
esac

echo
echo "The task has been successfully completed."
echo
