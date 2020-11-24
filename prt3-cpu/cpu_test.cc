#include <stdio.h>
#include <stdlib.h>

#include "cpu.h"
#include "../tut_src/test_lib.h"

#define VECTOR_LEN 128
#define USE_BRANCH false

unsigned globalSeed;

CPUInsn getFinishInsn() {
  union Insn insn;
  insn.generic.opcode = OPCODE_FINISH;
  return insn.generic;
}

CPUInsn getWriteImmediateInsn(int dst, int imm) {
  union Insn insn;
  insn.immediate.opcode = OPCODE_WR_IMM;
  insn.immediate.dst = dst;
  insn.immediate.imm = imm;
  return insn.generic;
}

CPUInsn getMemoryInsn(int opcode, int reg, int addr) {
  union Insn insn;
  insn.memory.opcode = opcode;
  insn.memory.reg = reg;
  insn.memory.addr = addr;
  return insn.generic;
}

CPUInsn getBranchInsn(int opcode, int src_0, int src_1, int new_pc) {
  union Insn insn;
  insn.branch.opcode = opcode;
  insn.branch.src_0 = src_0;
  insn.branch.src_1 = src_1;
  insn.branch.new_pc = new_pc;
  return insn.generic;
}

CPUInsn getBinaryInsn(int opcode, int dst, int src_0, int src_1) {
  union Insn insn;
  insn.binary.opcode = opcode;
  insn.binary.dst = dst;
  insn.binary.src_0 = src_0;
  insn.binary.src_1 = src_1;
  return insn.generic;
}

int main(void) {

  // Test outcome
  bool correct = true;

  // Test vector initialization
  int a[VECTOR_LEN];
  int b[VECTOR_LEN];
  int c[VECTOR_LEN];
  for (int i = 0; i < VECTOR_LEN; i++) {
    a[i] = static_cast<int>(rand_r(&globalSeed) % 1024 - 512);
    b[i] = static_cast<int>(rand_r(&globalSeed) % 1024 - 512);
    c[i] = 0;
  }

  // Input and output array initialization
  insn_T *i_mem = (insn_T *) allocBuffer(sizeof(insn_T) * DATA_SIZE);
  data_T *d_mem = (data_T *) allocBuffer(sizeof(data_T) * INSN_SIZE);
  for (int i = 0; i < DATA_SIZE; i++) {
    i_mem[i] = getFinishInsn();
  }
  for (int i = 0; i < INSN_SIZE; i++) {
    d_mem[i] = 0xDEADBEEF;
  }

  // Initialize Data Memory With Array Values
  int addr_A = 0;
  int addr_B = addr_A + VECTOR_LEN;
  int addr_C = addr_B + VECTOR_LEN;
  for (int i = 0; i < VECTOR_LEN; i++) {
    d_mem[addr_A+i] = (data_T) a[i];
    d_mem[addr_B+i] = (data_T) b[i];
    d_mem[addr_C+i] = (data_T) c[i];
  }

  // Assembly Program
#if USE_BRANCH == false
  for (int i = 0; i < VECTOR_LEN; i++) {
    i_mem[7*i+0] = getWriteImmediateInsn(R0, addr_A+i);   // Write A element address in R0
    i_mem[7*i+1] = getWriteImmediateInsn(R1, addr_B+i);   // Write B element address in R1
    i_mem[7*i+2] = getWriteImmediateInsn(R2, addr_C+i);   // Write C element address in R2
    i_mem[7*i+3] = getMemoryInsn(OPCODE_LOAD, R3, R0);    // Load A element into R3
    i_mem[7*i+4] = getMemoryInsn(OPCODE_LOAD, R4, R1);    // Load B element into R4
    i_mem[7*i+5] = getBinaryInsn(OPCODE_ADD, R5, R3, R4); // R5 <- R3 + R4
    i_mem[7*i+6] = getMemoryInsn(OPCODE_STORE, R5, R2);   // Store R5 into C
  }
#else
  // Loop Init
  i_mem[0] = getWriteImmediateInsn(R0, 0);            // loop index
  i_mem[1] = getWriteImmediateInsn(R1, 1);            // loop increment
  i_mem[2] = getWriteImmediateInsn(R2, VECTOR_LEN);   // loop bound
  i_mem[3] = getWriteImmediateInsn(R4, addr_A);       // A base address
  i_mem[4] = getWriteImmediateInsn(R5, addr_B);       // B base address
  i_mem[5] = getWriteImmediateInsn(R6, addr_C);       // C base address
  // TODO: Complete Loop Body
  i_mem[6] = ...
  // Loop Index Increment & Condition
  i_mem[...] = getBinaryInsn(OPCODE_ADD, R0, R0, R1); // increment loop index
  i_mem[...] = getBranchInsn(OPCODE_BNE, R0, R2, 6); // branch if not equal
#endif  // USE_BRANCH

  // Invoke the CPU
#if NO_SIM
  uint64_t t_fpga = cpu(i_mem, d_mem);
  printf("Synchronization time: %.3lfms\n", static_cast<float>(t_fpga) / 1E6);
#else
  cpu(i_mem, d_mem);
#endif

  // Copy value from data memory back to the arrays
  for (int i = 0; i < VECTOR_LEN; i++) {
    a[i] = (int) d_mem[addr_A+i];
    b[i] = (int) d_mem[addr_B+i];
    c[i] = (int) d_mem[addr_C+i];
  }

  // Check the output
  for (int i = 0; i < VECTOR_LEN; i++) {
    if (c[i] != a[i] + b[i]) {
      printf("Error at index %d, got %d but expected %d\n", i, c[i], a[i] + b[i]);
      correct = false;
    }
  }

  // Free arrays
  freeBuffer(i_mem);
  freeBuffer(d_mem);

  if (correct) {
    printf("Test successful\n");
    return 0;
  } else {
    printf("Test unsuccessful\n");
    return -1;
  }
}

