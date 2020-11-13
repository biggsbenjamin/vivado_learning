#include "add_ex.h"

int add_ex(int in_a, int in_b)
{
#pragma HLS INTERFACE s_axilite port=in_b

    int res;

    res = in_a + in_b;

    return res;
}
