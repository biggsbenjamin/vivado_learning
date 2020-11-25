#include <ap_axi_sdata.h>

#include "./cpu.h"

// Instruction cache index
typedef ap_uint<LOG_INSN_SIZE> insn_idx_T;
// Data cache index
typedef ap_uint<LOG_DATA_SIZE> data_idx_T;
// Register file index
typedef ap_uint<REGFILE_SIZE> rf_idx_T;

// Opcode type
typedef ap_uint<OPCODE_W> opcode_T;
// Immediate type
typedef ap_int<IMM_W> imm_T;

void cpu(
  volatile insn_T *i_mem,
  volatile data_T *d_mem
  ) {
#pragma HLS INTERFACE m_axi port = i_mem offset = slave bundle = i_mem depth = 1024
#pragma HLS INTERFACE m_axi port = d_mem offset = slave bundle = d_mem depth = 1024
#pragma HLS INTERFACE s_axilite port = return bundle = CONTROL_BUS

  // Instruction cache
  insn_T i_cache[INSN_SIZE];
  // Data cache
  data_T d_cache[DATA_SIZE];
  // Register file
  data_T reg_file[REGFILE_SIZE];

  // Program counter
  insn_idx_T pc = 0;

  // Finish flag (indicates program completion)
  bool finish = false;

  // Start by loading in the instruction and data memory into the caches
  memcpy(i_cache, const_cast<insn_T*>(i_mem), sizeof(insn_T) * INSN_SIZE);
  memcpy(d_cache, const_cast<data_T*>(d_mem), sizeof(data_T) * DATA_SIZE);

  // Now execute program until we encounter a FINISH opcode
  PROGRAM_LOOP: while (!finish) {
#pragma HLS PIPELINE
    // Fetch
    Insn insn;
    insn.generic = i_cache[pc];
    pc += 1;

    // Decode instruction fields
    opcode_T opcode = insn.generic.opcode;
    rf_idx_T dst = 0;
    rf_idx_T src_0 = 0;
    rf_idx_T src_1 = 0;
    insn_idx_T new_pc = 0;
    imm_T imm = 0;
    if (opcode == OPCODE_FINISH) {
      finish = true;
    } else if (opcode == OPCODE_WR_IMM) {
      dst = insn.immediate.dst;
      imm = insn.immediate.imm;
    } else if (opcode == OPCODE_LOAD) {
      dst = insn.memory.reg;
      src_0 = insn.memory.addr;
    } else if (opcode == OPCODE_STORE) {
      src_0 = insn.memory.addr;
      src_1 = insn.memory.reg;
    } else if (opcode == OPCODE_BEQ || opcode == OPCODE_BNE) {
      src_0 = insn.branch.src_0;
      src_1 = insn.branch.src_1;
      new_pc = insn.branch.new_pc;
    } else if (opcode == OPCODE_ADD) {
      dst = insn.binary.dst;
      src_0 = insn.binary.src_0;
      src_1 = insn.binary.src_1;
    } else {
      finish = true;
    }

    // Execute
    data_T arg_0 = reg_file[src_0];
    data_T arg_1 = reg_file[src_1];
    data_T res = 0;
    if (opcode == OPCODE_WR_IMM) {
      res = (data_T) imm;
    } else if (opcode == OPCODE_ADD) {
      res = (data_T) arg_0 + arg_1;
    }
    
    // TODO: add branch handling code here
    if (opcode == OPCODE_BEQ && arg_0 == arg_1) {
        pc = new_pc;
    }
    if (opcode == OPCODE_BNE && arg_0 != arg_1) {
        pc = new_pc;
    }

    // Memory load/store
    if (opcode == OPCODE_LOAD) {
      res = d_cache[arg_0];
    } else if (opcode == OPCODE_STORE) {
      d_cache[arg_0] = arg_1;
    }

    // Write Back
    if (opcode == OPCODE_WR_IMM  || opcode == OPCODE_LOAD || opcode == OPCODE_ADD) {
      reg_file[dst] = res;
    }
  }

  // Finish by storing the data cache back to data memory
  memcpy(const_cast<data_T*>(d_mem), const_cast<data_T*>(d_cache), sizeof(data_T) * DATA_SIZE);

}

