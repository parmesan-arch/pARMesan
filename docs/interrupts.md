# pARMesan Documentation

## Interrupts

The pARMesan ISA supports interrupts and exceptions. Internally, both are handled the same way. Semantically, the difference is that exceptions are triggered internally, such as from a processor error state or undefined instruction, while interrupts are triggered externally, from a processor interrupt pin. There is an IRQ pin for general interrupts (which can be masked) as well as an NMI pin for non-maskable interrupts. Additionally, interrupts can be triggered by the `INT` instruction.

### Interrupt Vector Table

Similar to the x86 architecture, we have an Interrupt Vector Table (IVT) that stores a list of pointers to interrupt handlers. When an interrupt or exception is handled, the processor determines the appropriate interrupt vector and jumps to it. These vectors are user-settable, and in fact must be defined by the user (or OS) in order for correct operation. The IVT is pointed to by a register, the IVTR. It is initialized to `0xF000`. The IVT is 256 bytes in size and stores 128 entries (2 bytes each). 

TODO: How to update the IVTR?

### Interrupt Handling Process

The first step is different for exceptions and interrupts. For an interrupt, a processor interrupt pin (IRQ or NMI) is triggered. If interrupts are disabled, IRQs are not handled. Otherwise, the processor reads a byte from memory address `0xDFFF`, the final byte in the MMIO space of IO device 255. This corresponds with the PIC's interrupt vector readout. This interrupt number is stored in the Interrupt Number Register, where it will be used in the more general exception handler.

In the case of an exception or a software-triggered interrupt, the interrupt number is already known, so this step is not necessary. The Interrupt Number Register would have already been set by the excepting instruction. Exceptions cannot be masked.

After these steps are complete, the remaining steps are common to both cases. The processor stores the current value of the Program Counter and the current flags (PSW) to the stack. It then reads the IVT entry, and jumps to that location to begin executing the interrupt handler. The processor also clears the interrupt pending flag to ensure that more interrupts are not executed. Interrupt handlers can choose to reenable interrupts using the EI instruction.

To return from an interrupt, a simple RET instruction is insufficient as the return address is not stored in the Link Register. Instead a RETI instruction must be used which restores the PC from the stack as well as the PSW from the stack.

---

[Table of Contents](index.md)