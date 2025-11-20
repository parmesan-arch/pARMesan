# pARMesan Documentation

## Instructions
WORK IN PROGRESS--Temporary Article

In general, instructions fall into a few categories: ALU, Data Movement, Memory, and Control Transfer. Live document with some more information can be found [here](https://docs.google.com/document/d/1DgnzGgg6i8BBU9w-XBLziPraKPNcnVzqKxEwDsCbySo)

## ALU Instructions
ALU operations have Register-Register variants and Register-Immediate variants. Instructions that take an immediate are limited to 4-bit immediates.

#### Register-Register ALU Instructions:
- ADD  Rd, Rs
- SUB  Rd, Rs
- AND  Rd, Rs
- OR   Rd, Rs
- XOR  Rd, Rs
- CMP  Rd, Rs
- LSL  Rd, Rs
- LSR  Rd, Rs
- ADC  Rd, Rs
- SBC  Rd, Rs
- TEST Rd, Rs
- ASR  Rd, Rs

#### Register-Immediate ALU Instructions: 
- ADD  Rd, #imm4
- SUB  Rd, #imm4
- AND  Rd, #imm4
- OR   Rd, #imm4
- XOR  Rd, #imm4
- CMP  Rd, #imm4
- LSL  Rd, #imm4
- LSR  Rd, #imm4

## Data Movement

### Register Moves

There are 3 Data Movement instructions. Two are used to load the low and high byte of a register, respectively. The last is used to transfer data between registers.
- MOVL Rd, #imm8
- MOVH Rd, #imm8
- MOV  Rd, Rs

### Memory
Memory can be addressed through three methods: Base + Offset, Pre-Index, and Post-Index.

In Base + Offset indexing, the value inside the base register is added to the immediate, and this is used as the address.

In Pre-Indexed, the same address calculation occurs, but the calculated address is written into the base register (overwriting the base address).

In Post-Indexed, the base address is used as the memory address, but the calculated address (with the offset added in) is written back into the base register.

Memory is accessed in granularity of Bytes or 16-bit Words.

- LOADW  `[mem]`
- LOADB  `[mem]`
- STOREW `[mem]`
- STOREB `[mem]`

An important caveat for memory addressing: Only registers `%sp` and `%ix` can be used in Base + Offset addressing. Pre- and Post-Indexed modes can use any register as the base.

## Control Transfer
There are a few options for control transfer. Jumps and Calls can be direct (to a label) or indirect (to a register). Conditional jumps can only be direct.
- JMP  .LABEL
- JMP  Rd
- J.CC .LABEL
- CALL .LABEL
- CALL Rd
- RET

### Miscellaneous
These are miscellaneous instructions that do not match any of the prior categories.
- HALT
- NOP



---

[Table of Contents](index.md)