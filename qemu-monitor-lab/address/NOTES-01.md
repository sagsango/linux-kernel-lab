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

