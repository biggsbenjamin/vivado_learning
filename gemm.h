#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef PART2_GEMM_H_
#define PART2_GEMM_H_

#define M 64
#define N 64
#define O 64

#ifndef NO_SIM
void gemm(
  volatile int *a,
  volatile int *b,
  volatile int *c);
#endif  // NO_SIM


#endif  // PART2_GEMM_H_
