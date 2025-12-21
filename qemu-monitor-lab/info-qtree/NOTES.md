QEMU device objects
    ↓ (info qtree)
Firmware ACPI namespace (DSDT/SSDT)
    |   (x86:acpidump, arm:/proc/device-tree)
    |   (x86: ACPI DSDT/SSDT | ARM: DTB)
    |   ls /sys/firmware/acpi/tables
    |   root@ubuntu-lkp:~# tree -d /sys/firmware/
    |    /sys/firmware/
    |    ├── acpi
    |    │   ├── hotplug
    |    │   │   ├── pci_root
    |    │   │   └── processor
    |    │   ├── interrupts
    |    │   └── tables
    |    │       ├── data
    |    │       └── dynamic
    |    ├── dmi
    |    │   └── tables
    |    └── memmap
    |    ├── 0
    |    ├── 1
    |    ├── 2
    |    ├── 3
    |    ├── 4
    |    ├── 5
    |    └── 6
    ↓
root@ubuntu-lkp:~#
Linux device model 
    (/sys, drivers, /proc)
    Kernel objects (struct device / driver / bus)
        ↓
    sysfs (/sys)        → live device model
    procfs (/proc)     → summarized / legacy views
    debugfs            → internal / optional detail


root@ubuntu-lkp:~# ls
ls           lsblk        lsinitramfs  lslogins     lsns         lspgpot
lsattr       lscpu        lsipc        lsmem        lsof         lspower
lsb_release  lshw         lslocks      lsmod        lspci        lsusb
