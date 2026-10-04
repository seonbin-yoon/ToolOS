#!/usr/bin/env bash

function error_check() {
    if [[ $1 -ne 0 ]]; then
        echo
        echo "A problem occurred while the task was running."
        exit $1
    fi
}

cd $(cd $(dirname ${BASH_SOURCE}) && pwd)/../ # To Source Tree Root
CONF_FILE="global.cfg"

if [[ ! -f ${CONF_FILE} ]]; then
    echo "ERR: '${CONF_FILE}' file does not exist." >&2
    exit 1
fi

eval "$(sed -e '/^\[/d' -e 's/ *#.*//' -e '/^$/d' "${CONF_FILE}")"

if [[ ! "${BUILD_THREADS}" =~ ^[0-9]+$ ]]; then
    BUILD_THREADS=$(nproc)
fi

if (( $# == 0 )); then
    echo
    echo " q   : Running QEMU (with gdb-multiarch)"
    echo " b   : BootLoader Build"
    echo " kb  : Kernel Build"
    echo " kb  : Binary Merge"
    echo
    read -p "> " CMDS
else
    CMDS="$*"
fi

clear
for CMD in ${CMDS}
do
    case "${CMD}" in
        q)
            echo "[Running QEMU]"
            echo "CPU: ${QEMU_CPU_CORE} Core"
            echo "MEM: ${QEMU_MEM}"
            echo "QEMU booted with the -s and -S options."

            cd "${QEMU_WORKSPACE}"
            qemu-system-aarch64 \
            -bios /usr/share/qemu-efi-aarch64/QEMU_EFI.fd \
            -machine virt \
            -cpu cortex-a710 \
            -smp cores=${QEMU_CPU_CORE},threads=1 \
            -m ${QEMU_MEM} \
            -drive file=fat:rw:hda-contents,if=none,format=raw,id=hd0 \
            -device virtio-blk-pci,drive=hd0,bootindex=0 \
            -device virtio-gpu-pci \
            -serial stdio \
            -net none \
            -s -S
            ;;
        b)
            echo "[BootLoader Build]"
            export PACKAGES_PATH="${EDK2_SRC}:${OS_SRC}"
            # [Python Script] [.inf file Location] [Source Tree Root] ... [Source code path]
            python3 "${OS_SRC}"/scripts/edk2_sources.py "${OS_SRC}"/boot/edk2_settings/ToolOS.inf "${OS_SRC}" "${OS_SRC}"/boot "${OS_SRC}"/include/boot
            error_check $?
            build -p boot/edk2_settings/ToolOS.dsc -m boot/edk2_settings/ToolOS.inf -a AARCH64 -t CLANGPDB -b DEBUG -n $((BUILD_THREADS * 2 + 1))
            error_check $?
            cp -v "${EDK2_SRC}"/Build/ToolOS/DEBUG_CLANGPDB/AARCH64/BOOTAA64.efi "${QEMU_WORKSPACE}"/hda-contents/EFI/BOOT/BOOTAA64.EFI
            error_check $?
            echo
            ;;
        kb)
            echo "[Kernel Build (ToolOS.elf)]"
            make -j${BUILD_THREADS}
            error_check $?
            cp -v ToolOS.elf "${QEMU_WORKSPACE}"/hda-contents/ToolOS.elf
            error_check $?
            echo
            ;;
        *)
            echo "ERR: Invalid command: ${CMD}"
            ;;
    esac
done

echo "The task has been successfully completed."
