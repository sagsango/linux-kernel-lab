# info mtree : QEMU sees and routes all guest memory and I/O accesses.
info-mtree ❱❱❱ grep -r "address-space\|memory-region"
info-mtree.out:address-space: VGA
info-mtree.out:address-space: e1000
info-mtree.out:address-space: cpu-memory-0
info-mtree.out:address-space: cpu-memory-1
info-mtree.out:address-space: memory
info-mtree.out:address-space: PIIX3
info-mtree.out:address-space: PIIX4_PM
info-mtree.out:address-space: e1000
info-mtree.out:address-space: i440FX
info-mtree.out:address-space: I/O
info-mtree.out:address-space: KVM-SMRAM
info-mtree.out:address-space: piix3-ide
info-mtree.out:memory-region: system
info-mtree.out:memory-region: pc.ram
info-mtree.out:memory-region: pc.bios
info-mtree.out:memory-region: pci
