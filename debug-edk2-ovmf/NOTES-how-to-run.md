# What we want 
Firmware: OVMF / EDK2
Bootloader: none
Kernel: none

# How to run ovmf
cd /home/sagar/groot/kernel/edk2/Build/OvmfX64/DEBUG_GCC5/FV
qemu-system-x86_64 \
  -machine q35,accel=kvm:tcg \
  -m 1024 \
  -cpu host \
  -drive if=pflash,format=raw,readonly=on,file=OVMF_CODE.fd \
  -drive if=pflash,format=raw,file=OVMF_VARS.fd \
  -serial stdio \
  -debugcon file:ovmf-debug.log \
  -global isa-debugcon.iobase=0x402 \
  -display none \
  -S -s



# How to attach gdb (with builtin scripts)
cd /home/sagar/groot/kernel/edk2/BaseTools/Scripts
Scripts ❱❱❱ ls
BinToPcd.py                  FormatDosFiles.py          RunMakefile.py
ConvertFceToStructurePcd.py  GccBase.lds                RunMakefileSample.mak
ConvertMasmToNasm.py         GetMaintainer.py           SetupGit.py
ConvertUni.py                GetUtcDateTime.py          SetVisualStudio.bat
DetectNotUsedItem.py         MemoryProfileSymbolGen.py  ShowEnvironment.bat
efi_debugging.py             PackageDocumentTools       SmiHandlerProfileSymbolGen.py
efi_gdb.py                   PatchCheck.py              UpdateBuildVersions.py
efi_lldb.py                  __pycache__
Scripts ❱❱❱ gdb -ex "target remote localhost:1234" -ex "source efi_gdb.py"
GNU gdb (Ubuntu 15.0.50.20240403-0ubuntu1) 15.0.50.20240403-git
Copyright (C) 2024 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.
Type "show copying" and "show warranty" for details.
This GDB was configured as "x86_64-linux-gnu".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<https://www.gnu.org/software/gdb/bugs/>.
Find the GDB manual and other documentation resources online at:
    <http://www.gnu.org/software/gdb/documentation/>.

For help, type "help".
Type "apropos word" to search for commands related to "word".
Remote debugging using localhost:1234
warning: No executable has been specified and target does not support
determining executable automatically.  Try using the "file" command.
0x000000000000fff0 in ?? ()
No symbol table is loaded.  Use the "file" command.
0x0000fff0 not in a PE/COFF (or TE) image
#0  0x000000000000fff0 in ?? ()
#1  0x0000000000000000 in ?? ()
(gdb) bt
#0  0x000000000000fff0 in ?? ()
#1  0x0000000000000000 in ?? ()
(gdb) help efi
Commands for debugging EFI. efi <cmd>

List of efi subcommands:

efi devicepath -- Display an EFI device path. Type 'efi devicepath -h' for more info
efi guid -- Display info about EFI GUID's. Type 'efi guid -h' for more info
efi hob -- Dump EFI HOBs. Type 'hob -h' for more info.
efi symbols -- Load Symbols for EFI. Type 'efi symbols -h' for more info.
efi table -- Dump EFI System Tables. Type 'table -h' for more info.

Type "help efi" followed by efi subcommand name for full documentation.
Type "apropos word" to search for commands related to "word".
Type "apropos -v word" for full documentation of commands related to "word".
Command name abbreviations are allowed if unambiguous.
(gdb)
