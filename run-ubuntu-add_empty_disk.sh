#!/bin/bash
QEMU_BIN=qemu-system-x86_64
NCPU=64
MEMSIZE=2G

KNL_SRC=/home/sagar/groot/kernel/linux-kernel-lab/linux     # TODO: Change with your kernel base location
BZIMAGE=${KNL_SRC}/arch/x86/boot/bzImage
CMDLINE="nokaslr console=ttyS0 root=/dev/sda1 rw debug"

# XXX: Adding empty cdrom does not work why?
#-drive file=my_cdrom.iso,media=cdrom,format=raw,readonly=on \
UBUNTU_IMG=noble-server-cloudimg-amd64.img
sudo ${QEMU_BIN} \
    -s \
    -nographic \
    -smp ${NCPU} -m ${MEMSIZE} \
    -nic user,host=10.0.2.10,hostfwd=tcp:127.0.0.1:2200-:22 \
    -net nic,model=e1000 \
    -drive file=${UBUNTU_IMG},format=qcow2 \
    -drive file=my_empty_disk.qcow2,format=qcow2,if=virtio \
    -enable-kvm \
    -kernel ${BZIMAGE} \
    -append "${CMDLINE}"

