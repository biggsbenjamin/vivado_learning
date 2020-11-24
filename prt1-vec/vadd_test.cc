#include <stdio.h>
#include <stdlib.h>

#include "vadd.h"
#include "../tut_src/test_lib.h"

unsigned globalSeed;

int main(void) {

  // Test outcome
  bool correct = true;

  // Input and output array initialization
  int *a = (int *) allocBuffer(sizeof(int) * VECTOR_LEN);
  int *b = (int *) allocBuffer(sizeof(int) * VECTOR_LEN);
  int *c = (int *) allocBuffer(sizeof(int) * VECTOR_LEN);
  for (int i = 0; i < VECTOR_LEN; i++) {
    a[i] = rand_r(&globalSeed) % 1024 - 512;
    b[i] = rand_r(&globalSeed) % 1024 - 512;
    c[i] = 0;
  }

  // Invoke the vector add module
#if NO_SIM
  uint64_t t_fpga = vadd(a, b, c);
  printf("Synchronization time: %.3lfms\n", static_cast<float>(t_fpga) / 1E6);
#else
  vadd(a, b, c);
#endif

  // Check the output
  for (int i = 0; i < VECTOR_LEN; i++) {
    if (c[i] != a[i] + b[i]) {
      correct = false;
    }
  }

  // Free arrays
  freeBuffer(a);
  freeBuffer(b);
  freeBuffer(c);

  if (correct) {
    printf("Test successful\n");
    return 0;
  } else {
    printf("Test unsuccessful\n");
    return -1;
  }
}

