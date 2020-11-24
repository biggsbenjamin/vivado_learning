#include <stdint.h>
#include <string.h>

#ifndef PART3_CPU_H_
#define PART3_CPU_H_

// Instruction width
#define INSN_W 32
// Data width
#define DATA_W 32

// Instruction depth
#define LOG_INSN_SIZE 10
#define INSN_SIZE (1 << LOG_INSN_SIZE)
// Data depth
#define LOG_DATA_SIZE 10
#define DATA_SIZE (1 << LOG_DATA_SIZE)
// Register file depth
#define LOG_REGFILE_SIZE 4
#define REGFILE_SIZE (1 << LOG_REGFILE_SIZE)

// Opcode width
#define OPCODE_W 3
// Immediate width
#define IMM_W 16

// Register Locations
#define R0  0
#define R1  1
#define R2  2
#define R3  3
#define R4  4
#define R5  5
#define R6  6
#define R7  7
#define R8  8
#define R9  9
#define R10 10
#define R11 11
#define R12 12
#define R13 13
#define R14 14
#define R15 15

// Opcodes
#define OPCODE_FINISH 0
#define OPCODE_WR_IMM 1
#define OPCODE_LOAD   2
#define OPCODE_STORE  3
#define OPCODE_BEQ    4
#define OPCODE_BNE    5
#define OPCODE_ADD    6

// Generic CPU instruction
typedef struct {
  uint32_t opcode         : OPCODE_W;
} CPUInsn;

// Write Immediate CPU instruction
// r[dst] <- imm
typedef struct {
  uint32_t opcode         : OPCODE_W;
  uint32_t dst            : LOG_REGFILE_SIZE;
  uint32_t imm            : IMM_W;
} ImmediateInsn;

// Memory CPU instruction
// LOAD:
//   r[reg] <- d_$(r[addr])
// STORE:
//   d_$(r[addr]) <- r[reg]
typedef struct {
  uint32_t opcode         : OPCODE_W;
  uint32_t reg            : LOG_REGFILE_SIZE;
  uint32_t addr           : LOG_REGFILE_SIZE;
} MemoryInsn;

// Branch CPU instruction
// BEQ:
//   if (r[src_0] == r[src_0])
//     pc <- new_pc
// BNE:
//   if (r[src_0] != r[src_0])
//     pc <- new_pc
typedef struct {
  uint32_t opcode         : OPCODE_W;
  uint32_t src_0          : LOG_REGFILE_SIZE;
  uint32_t src_1          : LOG_REGFILE_SIZE;
  uint32_t new_pc         : LOG_INSN_SIZE;
} BranchInsn;

// Binary CPU instruction
// ADD:
//   r[dst] <- r[src_0] + r[src_1]
typedef struct {
  uint32_t opcode         : OPCODE_W;
  uint32_t dst            : LOG_REGFILE_SIZE;
  uint32_t src_0          : LOG_REGFILE_SIZE;
  uint32_t src_1          : LOG_REGFILE_SIZE;
} BinaryInsn;

// Union to easily decode instructions
union Insn {
  CPUInsn generic;
  ImmediateInsn immediate;
  MemoryInsn memory;
  BranchInsn branch;
  BinaryInsn binary;
};

// Instruction width
typedef CPUInsn insn_T;

#ifndef NO_SIM
  #include <ap_int.h>

  // Data width
  typedef ap_int<DATA_W> data_T;

  // Simple CPU
  void cpu(
    volatile insn_T *i_mem,
    volatile data_T *d_mem
    );
#endif  // NO_SIM

#endif  // PART3_CPU_H_

