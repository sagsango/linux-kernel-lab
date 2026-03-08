#!/bin/bash

# Configuration
QEMU_BIN=qemu-system-x86_64
NCPU=2
MEMSIZE=2G
KNL_SRC="/home/sagar/groot/kernel/linux-kernel-lab/linux"
BZIMAGE="${KNL_SRC}/arch/x86/boot/bzImage"
UBUNTU_IMG="noble-server-cloudimg-amd64.img"

# Kernel Parameters
# added intel_iommu=on for VT-d and iommu=pt for performance (Pass-Through)
CMDLINE="nokaslr console=ttyS0 root=/dev/sda1 rw debug intel_iommu=on iommu=pt"

# Hardware Emulation Strategy:
# 1. machine q35: Required for PCIe and Intel-IOMMU
# 2. kernel-irqchip=split: Required for 'intremap=on'
# 3. -cpu host,-svm: Uses host CPU features but hides AMD-specific SVM to stop warnings
sudo ${QEMU_BIN} \
    -s \
    -nographic \
    -enable-kvm \
    -cpu host,-svm \
    -smp ${NCPU} \
    -m ${MEMSIZE} \
    -machine q35,accel=kvm,kernel-irqchip=split \
    -device intel-iommu,intremap=on,device-iotlb=on \
    -kernel "${BZIMAGE}" \
    -drive file="${UBUNTU_IMG}",format=qcow2,if=virtio \
    -append "${CMDLINE}" \
    -nic user,model=virtio-net-pci,hostfwd=tcp:127.0.0.1:2200-:22
