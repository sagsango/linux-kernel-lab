# Press the keyboard key on the top of the vm (no-virtio)
══════════════════════════════════════════════════════════════════════
                 KEY PRESS FLOW — QEMU + KVM + LINUX
══════════════════════════════════════════════════════════════════════

[ Physical Keyboard ]
        |
        v
[ USB / PS2 Controller ]
        |
        v
======================== HOST KERNEL ========================
        |
        v
IRQ → usb_hcd_irq() / i8042_interrupt()
        |
        v
HID / AT keyboard driver
        |
        v
input_report_key(KEY_A)
        |
        v
input_event()
        |
        v
/dev/input/eventX
======================== HOST USERSPACE =====================
        |
        v
QEMU frontend (SDL/GTK/evdev)
        |
        v
qemu_input_event_send_key()
        |
        v
Emulated Keyboard Device
  (i8042 / USB / virtio-input)
        |
        v
=========================== KVM =============================
        |
        v
ioctl(KVM_INTERRUPT)
        |
        v
Inject virtual IRQ
========================= GUEST CPU =========================
        |
        v
IDT[IRQ1]
        |
        v
common_interrupt
        |
        v
do_IRQ()
        |
        v
handle_irq_event()
        |
        v
i8042_interrupt()
======================== GUEST KERNEL =======================
        |
        v
atkbd.c
        |
        v
input_report_key()
        |
        v
/dev/input/eventX
        |
        v
TTY line discipline (n_tty)
        |
        v
====================== GUEST USERSPACE ======================
        |
        v
Shell / Application
        |
        v
read(STDIN) → 'A'
══════════════════════════════════════════════════════════════════════

# Press the keyboard key on the top of the vm (virtio)
═══════════════════════════════════════════════════════════════════════
           KEYPRESS FLOW — QEMU + KVM + VIRTIO-INPUT
═══════════════════════════════════════════════════════════════════════

[ Physical Keyboard ]
        |
        v
[ USB / HID Controller ]
        |
        v
=========================== HOST KERNEL ===============================
        |
        v
IRQ → usb_hcd_irq()
        |
        v
HID keyboard driver
        |
        v
input_report_key(KEY_A)
        |
        v
/dev/input/eventX
========================== HOST USERSPACE =============================
        |
        v
QEMU input core
        |
        v
virtio-input backend
        |
        v
Fill virtqueue:
  struct virtio_input_event
        |
        v
============================ PCI BUS =================================
        |
        v
MSI-X interrupt
============================== KVM ===================================
        |
        v
KVM_SIGNAL_MSI
=========================== GUEST CPU ================================
        |
        v
IDT[MSI vector]
        |
        v
common_interrupt
        |
        v
handle_irq_event()
========================== GUEST KERNEL ===============================
        |
        v
virtio_input_irq()
        |
        v
virtqueue_get_buf()
        |
        v
input_report_key(KEY_A)
        |
        v
/dev/input/eventX
        |
        v
TTY line discipline (n_tty)
========================= GUEST USERSPACE =============================
        |
        v
Shell / Application
        |
        v
read(STDIN) → 'A'
═══════════════════════════════════════════════════════════════════════

