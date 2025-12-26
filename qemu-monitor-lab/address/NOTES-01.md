# mappings
Guest Virtual Address (GVA)
        ↓  (guest page tables)
Guest Physical Address (GPA)
        ↓  (EPT / NPT)
Host Virtual Address (HVA)
        ↓  (host page tables)
Host Physical Address (HPA)


# who knows what
QEMU knows:
  GPA → HVA

KVM knows:
  GPA → HPA

Guest knows:
  GVA → GPA


# dump GPA → HVA
- qemu monitor
    (qemu) info mem


# read the va->pa mapping
cat /proc/<pid>/pagemap
and there are tools to process this file


# gpa2hva addr
Print the host virtual address at which the guest’s physical address addr is mapped.
# gpa2hpa addr
Print the host physical address at which the guest’s physical address addr is mapped.
# gva2gpa addr
Print the guest physical address at which the guest’s virtual address addr is mapped based on the mapping for the current CPU.


(qemu) gpa2hva 0x1000
gpa2hva 0x1000
Host virtual address for 0x1000 (pc.ram) is 0x76f607e01000
(qemu)
(qemu) gpa2hpa 0x1000
gpa2hpa 0x1000
Host physical address for 0x1000 (pc.ram) is 0x15b2ca000
(qemu)
(qemu) gva2gpa 0x1000
gva2gpa 0x1000
Unmapped
(qemu) gva2gpa 0x000000ffffffff
gva2gpa 0x000000ffffffff
Unmapped
(qemu)
(qemu) gva2gpa 0xffffff000000000
gva2gpa 0xffffff000000000
Unmapped
