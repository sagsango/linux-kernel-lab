#!/bin/bash
QEMU_BIN=qemu-system-x86_64
NCPU=2
MEMSIZE=2G

KNL_SRC=/home/sagar/groot/kernel/linux-kernel-lab/linux     # TODO: Change with your kernel base location
BZIMAGE=${KNL_SRC}/arch/x86/boot/bzImage
CMDLINE="nokaslr console=ttyS0 root=/dev/sda1 rw debug"

UBUNTU_IMG=noble-server-cloudimg-amd64.img

sudo ${QEMU_BIN} \
    -s \
    -nographic \
    -trace file=trace.log \
    -monitor tcp:127.0.0.1:4444,server,nowait \
    -smp ${NCPU},sockets=1,cores=8,threads=1,maxcpus=8 \
    -m ${MEMSIZE} \
    -nic user,host=10.0.2.10,hostfwd=tcp:127.0.0.1:2200-:22 \
    -net nic,model=e1000 \
    -drive file=${UBUNTU_IMG},format=qcow2 \
    -enable-kvm \
    -kernel ${BZIMAGE} \
    -append "${CMDLINE}"
