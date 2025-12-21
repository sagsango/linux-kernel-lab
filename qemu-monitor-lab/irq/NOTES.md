# Mental model (lock this in)
Device asserts interrupt
 → QEMU raises IRQ line
   → KVM injects IRQ
     → (info irq counter++)
       → Guest CPU receives interrupt
         → Linux handler runs
           → (/proc/interrupts counter++)
So:
info irq = injections
/proc/interrupts = handled


# When to use which (debugging guide)
Symptom	                            Check
Device silent	                    info irq
IRQ injected but no handler	        /proc/interrupts
Driver bug	                        /proc/interrupts
QEMU/KVM wiring bug	                info irq
Lost interrupts	Compare             both
