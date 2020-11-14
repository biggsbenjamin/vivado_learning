#ifndef TEST_LIB_H_
#define TEST_LIB_H_

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef NO_SIM


  #include "./pynq_driver.h"
  #include "../part3/cpu.h"

  // CPU data width
  typedef int data_T;

  uint64_t vadd(int *a, int *b, int *c);
  uint64_t gemm(int *a, int *b, int *c);
  uint64_t cpu(insn_T *i_mem, data_T *d_mem);

#endif  // NO_SIM

/*!
* \brief Performs memory allocation in a physically contiguous region of memory.
* \param num_bytes Size of the buffer in bytes.
* \return Pointer to the allocated buffer.
*/
void * allocBuffer(size_t num_bytes);

/*!
* \brief Frees buffer allocated in a physically contiguous region of memory.
* \param buffer Pointer to the buffer to free.
*/
void freeBuffer(void * buffer);


#endif  //  TEST_LIB_H_

