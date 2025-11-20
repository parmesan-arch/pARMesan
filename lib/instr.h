#ifndef _INSTR_H_
#define _INSTR_H_

#include <stdint.h>
#include <stdbool.h>

#define INSTRUCTION_WIDTH 2

typedef enum opcode {
  ERR = 0,
  CHGSTAT,
  ALU_RR,
  ALU_RI,
  FLOAT,
  LDWPRE,
  LDWPOST,
  LDWSPIX,
  LDBPRE,
  LDBPOST,
  LDBSPIX,
  MOVL,
  STWPRE,
  STWPOST,
  STWSPIX,
  STBPRE,
  STBPOST,
  STBSPIX,
  MOVH,
  MOV,
  RET,
  JCC,
  INT,
  JMP,
  JMPR,
  CALL,
  CALLR,
  HLT,
  EI,
  DI,
  NOP,
  ADD,
  SUB,
  AND,
  OR,
  XOR,
  CMP,
  LSL,
  LSR,
  ADC,
  SBC,
  TEST,
  ASR,
  // MUL,
  // DIV,
  IADD,
  ISUB,
  IAND,
  IOR,
  IXOR,
  ICMP,
  ILSL,
  ILSR
} opcode_t;

typedef enum alu_op {
  ALU_ERR = 0,
  ALU_PLUS,
  ALU_MINUS,
  ALU_AND,
  ALU_OR,
  ALU_XOR,
  ALU_LSL,
  ALU_LSR,
  ALU_ASR,
  ALU_MUL,
  ALU_DIV,
  ALU_ADC,
  ALU_SBC,
  ALU_MOVL,
  ALU_MOVH,
  ALU_PASS_B,
  ALU_PASS_A,
  ALU_NONE
} alu_op_t;

typedef struct ctrl_sigs {
  // consumed in Decode

  // if true, val_a comes from src, else from dst
  // this is the case only in load/store instrs, as
  // the ALU source is not the same as the data source/dst
  bool val_a_sel;

  bool val_b_is_imm; // if true, ALU second operand is immediate
  bool call;

  // consumed in Execute
  bool set_cc;

  // consumed in Memory
  bool mem_read;
  bool mem_write;
  bool is_word;
  bool address_from_execute;

  // consumed in Writeback
  bool wval_1_src; // Choose where to get primary wval from. If true, get from
                   // ALU. If false, get from memory output.
  bool w_enable_1;
  bool w_enable_2;
} ctrl_sigs_t;

typedef enum condition_code { EQ, NE, GE, GT, LT, LE, CS, CC } condition_code_t;

typedef struct instr {
  // generated in Fetch
  uint16_t insnbits;

  // generated in Decode
  opcode_t op;            // opcode of the instruction
  uint16_t mem_writeval;  // value to be written to mem
  uint16_t mem_bytewidth; // how wide (in bytes) the read/write is (only
                          // applicable to loads or stores)
  uint16_t opnd_1;        // ALU's first value
  uint16_t opnd_2;        // ALU's second value
  alu_op_t alu_op;        // ALU operation to perform
  condition_code_t cond;  // condition code for ALU to check
  int dst1;               // first writeback destination
  int dst2;               // second writeback destination
  uint16_t branch_pc;     // next instruction address in case of a branch (otherwise
                          // it's just sequential successor)
  ctrl_sigs_t ctrl_sigs;  // control signals to be used for this instr

  // generated in Execute
  uint16_t ex_val; // value produced from ALU
  bool cond_holds; // condition holds or not

  // generated in Memory
  uint16_t mem_readval; // value read from memory

} instr_t;

/**
 * Lookup table for top-level opcodes.
 * A mapping from the first 5 bits of an instruction to the corresponding
 * opcode. Also called an "itable". Some of these instructions are placeholders
 * and must be resolved with resolve_opcode, like "CHGSTAT" or "ALU_RR"
 */
static const opcode_t TOPLEVEL_LOOKUP[32] = {
    CHGSTAT, ALU_RR,  ALU_RI,  FLOAT, LDWPRE, LDWPOST, LDWSPIX, MOVL,
    STWPRE,  STWPOST, STWSPIX, MOVH,  LDBPRE, LDBPOST, LDBSPIX, ERR,
    STBPRE,  STBPOST, STBSPIX, ERR,   JCC,    MOV,     INT,     ERR,
    JMP,     JMPR,    CALL,    CALLR, ERR,    ERR,     ERR,     ERR};

/**
 * Lookup table to resolve CHGSTAT instructions.
 */
static const opcode_t CHGSTAT_LOOKUP[8] = {HLT, RET, ERR, EI, DI, ERR, ERR, NOP};

/**
 * Lookup table to resolve ALU_RR instructions.
 */
static const opcode_t ALU_RR_LOOKUP[16] = {ADD, SUB, AND, OR, XOR, CMP, LSL, LSR,
                                           ADC, SBC, TEST, ASR, ERR, ERR, ERR, ERR};

/**
 * Lookup table to resolve ALU_RI instructions.
 */
static const opcode_t ALU_RI_LOOKUP[8] = {IADD, ISUB, IAND, IOR,
                                          IXOR, ICMP, ILSL, ILSR};

/**
 * Lookup table for bitmasks used in immediate logical ops.
 */
static const uint16_t BITMASK_LOOKUP[32] = {
  0x0000, 0x1111, 0x2222, 0x3333, 0x4444, 0x5555, 0x6666, 0x7777,
  0x8888, 0x9999, 0xAAAA, 0xBBBB, 0xCCCC, 0xDDDD, 0xEEEE, 0xFFFF,
  0x00FF, 0xFF00, 0x0FF0, 0xF00F, 0x0F0F, 0xF0F0, 0x0000, 0x0000,
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
};

#endif