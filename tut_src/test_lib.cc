#include "./test_lib.h"

#ifdef NO_SIM

uint64_t vadd(int *a, int *b, int *c) {
  // Performance counter variables
  uint64_t t_fpga;
  struct timespec start, stop;

  // Program
  Program("vadd.bit");

  // HLS Design Handle
  void* hls_handle = MapRegister(HLS_IP_ADDR, RANGE);

  // Physical address pointers
  uint32_t a_phy = cma_get_phy_addr(a);
  uint32_t b_phy = cma_get_phy_addr(b);
  uint32_t c_phy = cma_get_phy_addr(c);

  clock_gettime(CLOCK_REALTIME, &start);

  WriteMappedReg(hls_handle, 0x10, a_phy);
  WriteMappedReg(hls_handle, 0x18, b_phy);
  WriteMappedReg(hls_handle, 0x20, c_phy);
  WriteMappedReg(hls_handle, 0x00, START);

  // Loop until done
  unsigned t, flag = 0;
  unsigned wait_cycles = 1000000;
  for (t = 0; t < wait_cycles; ++t) {
    flag = ReadMappedReg(hls_handle, 0x00);
    if (flag & DONE) break;
  }

  clock_gettime(CLOCK_REALTIME, &stop);
  t_fpga = 1000000000ULL * (stop.tv_sec - start.tv_sec) + (stop.tv_nsec - start.tv_nsec);

  return t_fpga;
}

uint64_t gemm(int *a, int *b, int *c) {
  // Performance counter variables
  uint64_t t_fpga;
  struct timespec start, stop;

  // Program
  Program("gemm.bit");

  // HLS Design Handle
  void* hls_handle = MapRegister(HLS_IP_ADDR, RANGE);

  // Physical address pointers
  uint32_t a_phy = cma_get_phy_addr(a);
  uint32_t b_phy = cma_get_phy_addr(b);
  uint32_t c_phy = cma_get_phy_addr(c);

  clock_gettime(CLOCK_REALTIME, &start);

  WriteMappedReg(hls_handle, 0x10, a_phy);
  WriteMappedReg(hls_handle, 0x18, b_phy);
  WriteMappedReg(hls_handle, 0x20, c_phy);
  WriteMappedReg(hls_handle, 0x00, START);

  // Loop until done
  unsigned t, flag = 0;
  unsigned wait_cycles = 1000000;
  for (t = 0; t < wait_cycles; ++t) {
    flag = ReadMappedReg(hls_handle, 0x00);
    if (flag & DONE) break;
  }

  clock_gettime(CLOCK_REALTIME, &stop);
  t_fpga = 1000000000ULL * (stop.tv_sec - start.tv_sec) + (stop.tv_nsec - start.tv_nsec);

  return t_fpga;
}

uint64_t cpu(insn_T *i_mem, data_T *d_mem) {
  // Performance counter variables
  uint64_t t_fpga;
  struct timespec start, stop;

  // Program
  Program("cpu.bit");

  // HLS Design Handle
  void* hls_handle = MapRegister(HLS_IP_ADDR, RANGE);

  // Physical address pointers
  uint32_t i_mem_phy = cma_get_phy_addr(i_mem);
  uint32_t d_mem_phy = cma_get_phy_addr(d_mem);

  clock_gettime(CLOCK_REALTIME, &start);

  WriteMappedReg(hls_handle, 0x10, i_mem_phy);
  WriteMappedReg(hls_handle, 0x18, d_mem_phy);
  WriteMappedReg(hls_handle, 0x00, START);

  // Loop until done
  unsigned t, flag = 0;
  unsigned wait_cycles = 1000000;
  for (t = 0; t < wait_cycles; ++t) {
    flag = ReadMappedReg(hls_handle, 0x00);
    if (flag & DONE) break;
  }

  clock_gettime(CLOCK_REALTIME, &stop);
  t_fpga = 1000000000ULL * (stop.tv_sec - start.tv_sec) + (stop.tv_nsec - start.tv_nsec);

  return t_fpga;
}



#endif  // NO_SIM

void * allocBuffer(size_t num_bytes) {
#ifdef NO_SIM
  return cma_alloc(num_bytes, CACHED);
#else
  return malloc(num_bytes);
#endif
}

void freeBuffer(void * buffer) {
#ifdef NO_SIM
  return cma_free(buffer);
#else
  return free(buffer);
#endif
}

