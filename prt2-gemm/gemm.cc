#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./gemm.h"

// This module performs matrix multiplication of matrices A and B
// Where A is an (m,n) and B is an (n,o) matrix.
// We assume that B is stored transposed, resulting in a (o,n) shape. 
void gemm(
    volatile int *a,
    volatile int *b,
    volatile int *c) {
#pragma HLS INTERFACE m_axi port = a offset = slave bundle = a_port depth = 64*64//axi4 are streaming protocols - can only go one way
#pragma HLS INTERFACE m_axi port = b offset = slave bundle = b_port depth = 64*64//read in
#pragma HLS INTERFACE m_axi port = c offset = slave bundle = c_port depth = 64*64//write out
#pragma HLS INTERFACE s_axilite port = return bundle = CONTROL_BUS

    int a_buff[M][N]; //these are buffers to store the streamed info
#pragma HLS array_partition variable=a_buff block factor=8 dim=2
    int b_buff[O][N];
#pragma HLS array_partition variable=b_buff block factor=8 dim=2
    int c_buff[M][O];
//#pragma HLS array_partition variable=c_buff complete dim=0 // this was a naive thing to do
#pragma HLS array_partition variable=c_buff block factor=8 dim=2

    // Load A & B
    memcpy(&a_buff[0][0], const_cast<int*>(a), sizeof(int) * M * N); //only works for a top level fn or arg
    memcpy(&b_buff[0][0], const_cast<int*>(b), sizeof(int) * O * N); //burst access from memory - pipelined, for buffer storage

    int temp_sum;

    // Matrix Multiplication
    for (int m = 0; m < M; m++) {
#pragma HLS PIPELINE 
        for (int o = 0; o < O; o ++) {
            //c_buff[m][o] = 0;
            temp_sum = 0;
            for (int n = 0; n < N; n++) {
                //c_buff[m][o] += a_buff[m][n] * b_buff[o][n];
                temp_sum += a_buff[m][n] * b_buff[o][n];
            }
            c_buff[m][o] = temp_sum;
        }
    }

    // Store C
    memcpy(const_cast<int*>(c), const_cast<int*>(&c_buff[0][0]), sizeof(int) * M * O);
}
