#include "vct.h"
#include "logging.h"

static FILE *out_file = NULL;

static bool modified_psw = 0;
static int modified_regs = 0;
static int modified_mems = 0;

static flags_t flags = {0};
static status_t status = STAT_OK;
static uint8_t reg_indices[8] = {0};
static uint16_t reg_values[8] = {0};
static uint16_t mem_locations[16] = {0};
static uint8_t mem_values[16] = {0};

void initialize_vct(FILE *out, flags_t initial_flags, status_t initial_status) {
  out_file = out;

  // reset data structures
  modified_psw = false;
  modified_regs = 0;
  modified_mems = 0;

  flags = initial_flags;
  status = initial_status;

  if (!do_vct) {
    return;
  }
  // write VCT header
  vct_header_t hdr = {"vct!", 1, 9, 0x00};
  fwrite(&hdr, sizeof(hdr), 1, out_file);
  char *reg_names[] = {"AX", "BX", "CX", "DX", "IX", "BP", "SP", "LR", "PC"};
  for (int i = 0; i < 9; i++) {
    fwrite(reg_names[i], 1, 3, out_file);
  }
  
  // write initial PSW
  uint16_t psw = PACK_PSW(flags, status);
  fwrite(&psw, sizeof(psw), 1, out_file);

  // write initial reg values
  for (int i = 0; i < 9; i++) {
    uint16_t val = 0;
    fwrite(&val, sizeof(val), 1, out_file);
  }
}

void track_flags(flags_t new_flags) {
  flags = new_flags;
  modified_psw = true;
}

void track_status(status_t new_status) {
  status = new_status;
  modified_psw = true;
}

void track_reg(uint8_t index, uint16_t value) {
  // if we've already tracked a change in this register, this cycle:
  for (int i = 0; i < modified_regs; i++) {
    if (reg_indices[i] == index) {
      reg_values[i] = value;
      return;
    }
  }

  // we don't already have a mapping for this reg change, so make one

  // we can only track 7 register changes
  if (modified_regs > 7) {
    write_log(LOG_WARN, "Attempted to track more than 7 register changes. Trying to track %d.", modified_regs);
    return;
  }
  reg_indices[modified_regs] = index;
  reg_values[modified_regs] = value;
  modified_regs++;
}

void track_mem(uint16_t address, uint8_t value) {
  for (int i = 0; i < modified_mems; i++) {
    if (mem_locations[i] == address) {
      mem_values[i] = value;
      return;
    }
  }

  // we don't already have a mapping for this memory change, so make one
  if (modified_mems > 15) {
    write_log(LOG_WARN, "Attempted to track more than 15 memory changes. Trying to track %d.", modified_mems);
    return;
  }
  mem_locations[modified_mems] = address;
  mem_values[modified_mems] = value;
  modified_mems++;
}

void finalize_cycle() {
  write_log(LOG_DEBUG, "finalizing");
  if (!do_vct) {
    modified_psw = false;
    modified_regs = 0;
    modified_mems = 0;
    return;
  }

  // enforce limits and log fatally instead of aborting via assert
  if (modified_regs > 7) {
    write_log(LOG_FATAL, "VCT: too many register changes in cycle: %d (max 7)", modified_regs);
  }
  if (modified_mems > 15) {
    write_log(LOG_FATAL, "VCT: too many memory changes in cycle: %d (max 15)", modified_mems);
  }
  // cycle header
  uint8_t cycle_header = (modified_psw ? 0x80 : 0x00) | (modified_regs << 4) | modified_mems;
  fwrite(&cycle_header, sizeof(cycle_header), 1, out_file);

  // flags header
  if (modified_psw) {
    uint16_t psw_packed = PACK_PSW(flags, status);
    fwrite(&psw_packed, sizeof(psw_packed), 1, out_file);
  }

  // write register changes
  for (int i = 0; i < modified_regs; i++) {
    fwrite(&(reg_indices[i]), sizeof(reg_indices[i]), 1, out_file);
    fwrite(&(reg_values[i]), sizeof(reg_values[i]), 1, out_file);
  }

  // write memory changes
  for (int i = 0; i < modified_mems; i++) {
    fwrite(&(mem_locations[i]), sizeof(mem_locations[i]), 1, out_file);
    fwrite(&(mem_values[i]), sizeof(mem_values[i]), 1, out_file);
  }

  // reset these
  modified_psw = false;
  modified_regs = 0;
  modified_mems = 0;
}