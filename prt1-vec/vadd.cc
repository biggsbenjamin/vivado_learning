#include "./vadd.h"

void vadd(
    volatile int *a,
    volatile int *b,
    volatile int *c) {

#pragma HLS INTERFACE m_axi port = a offset = slave bundle = a_port
#pragma HLS INTERFACE m_axi port = b offset = slave bundle = b_port
#pragma HLS INTERFACE m_axi port = c offset = slave bundle = c_port
#pragma HLS INTERFACE s_axilite port = return bundle = CONTROL_BUS

    // Vector add loop
    for (int i = 0; i < VECTOR_LEN; i++) {
#pragma HLS PIPELINE
        c[i] = a[i] + b[i];
    }
}
