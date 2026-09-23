#ifndef _PROC_H_
#define _PROC_H_

#include "instr.h"

#define INITIAL_IP 0xF000

typedef struct flags {
  bool V;
  bool C;
  bool Z;
  bool N;
  bool I;
  bool T;
} flags_t;

#define PACK_FLAGS(flags) ((flags.T << 5) | (flags.I << 4) | (flags.N << 3) | (flags.Z << 2) | (flags.C << 1) | (flags.V << 0))

#define PACK_PSW(flags, status) (PACK_FLAGS(flags) << 8 | status)
typedef enum status {
  STAT_OK,
  STAT_INTERRUPT,
  STAT_EXCEPTION,
  STAT_HALT,
  STAT_ERR
} status_t;


typedef enum registers {
  REG_AX,
  REG_BX,
  REG_CX,
  REG_DX,
  REG_IX,
  REG_BP,
  REG_SP,
  REG_LR,
  REG_PC
} registers_t;

typedef struct proc {
  uint16_t instruction_pointer;
  uint16_t gpr_file[8];
  // float fpr_file[8]; // might one day add floating-point support
  flags_t flags;
  status_t status;
  bool interrupt_pin;
  bool internal_interrupt;
  uint16_t interrupt_cause_register;
} proc_t;

/**
 * Construct and initialize a processor to default values.
 *
 * @return a new proc_t instance with default values.
 */
proc_t init_proc();

/** Simulate one iteration of the fetch stage of the pipeline. */
void fetch(proc_t *, instr_t *);

/** Simulate one iteration of the decode stage of the pipeline. */
void decode(proc_t *, instr_t *);

/** Simulate one iteration of the execute stage of the pipeline. */
void execute(proc_t *, instr_t *);

/** Simulate one iteration of the memory stage of the pipeline. */
void memory(proc_t *, instr_t *);

/** Simulate one iteration of the writeback stage of the pipeline. */
void writeback(proc_t *, instr_t *);

/** Simulate handling an interrupt. */
void handle_interrupt(proc_t *);

#endif