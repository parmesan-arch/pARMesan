# pARMesan Documentation

## Memory Map

This is a description of the layout of memory in a running pARMesan system. In reality, such a memory map would be defined by the operating system. However, as this project includes an operating system running on the pARMesan architecture, we will include our memory map description here.

## Summary

TODO: Determine the range for the IVT.

TBD: These  256 bytes contain the Interrupt Vector Table. Realistically, probably only the lower 32 interrupt vectors (64 bytes) will be used at any time. Still, this setup provides support for up to 128 different interrupts.

`0x0000` to `0x0FFF`: System BIOS ROM. This ROM contains all BIOS subroutines and system calls.

In our system, we will begin execution at `0x0000`. BIOS will be mapped to this area.

TODO: Update cheesegrater so that the start of code is 0x0000.

There will likely also be 4K of video memory, to be determined more exactly at a later date. This will probably start at `0xE000`. The video system will be similar to the Monochrome Display Adaptor from the IBM PC.

Some range in memory will be reserved for IO devices using MMIO. For the time being, this range is `0xD000` to `0xDFFF` (4K Bytes). Each MMIO device is granted 16 bytes of MMIO space. The PIC is defined as mapping to MMIO device number 255, with MMIO addresses ranging from `0xDFF0` to `0xDFFF`. This definition is system-level, not an intrinsic quality of the processor, so it can change.

---

[Table of Contents](index.md)