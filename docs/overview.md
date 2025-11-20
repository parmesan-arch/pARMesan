# pARMesan Documentation

## Overview
In a pARMesan CPU, there are 8 general-purpose registers, all 16 bits wide, and a 16-bit instruction pointer. The CPU can address 64k of ram as 64Kx8. The instruction set is detailed more in the ISA section, but in general, the ISA is designed to be a 16-bit general purpose instruction set designed with the RISC principles in mind. 

Any GPR can be used in an ALU operation or data movement operation. The first four, designated `%ax`, `%bx`, `%cx`, and `%dx`, are intended for general-purpose data usage. The other four, `%ix`, `%bp`, `%sp`, and `%lr`, are intended for more specific use as the Index Register, Base Pointer, Stack Pointer, and Link Register. The Link Register `%lr` is used in CALL and RET instructions to save the return address.

The processor's word size is 16 bits, meaning 64K addresses can be generated. Memory access is designed to be byte-addressed. Memory accesses are indexed in one of three ways, similar to AArch64: Offset, Pre-Indexed, and Post-Indexed. Offset (or Base + Offset) indexing simply calculates an address by adding a signed immediate to the value in a register (called the Base register). This mode is only available if the Index Register or Stack Pointer are used as the base. Pre-Indexed performs this same calculation, but writes the result back into the Base Register. Post-Indexed first uses the unmodified Base Register to index into memory, then performs the addition and writes the result back into the Base Register. 

Notably if the Index Register or Stack Pointer is used as the base register in a Base+Offset addressing operation, the bounds of the immediate are increased to 63 and -64.

An instruction cycle is made of 5 machine cycles, and each machine cycle performs a different subtask of one instruction. This follows the 5-stage Fetch, Decode, Execute, Memory, Writeback paradigm. The CPU supports a dual register writeback, which is required in the case of an instruction that reads from memory and modifies a pointer, such as a pre/post-indexed LOAD.

IO is handled throgh Memory Mapped IO, or MMIO. This also frees up more of the instruction encoding space for other instructions.

A Rust implementation of this CPU is planned, and a little work has been done towards that project, found in it's Git repo: [bytesize](https://github.com/Pritjam/bytesize).

---

[Table of Contents](index.md)