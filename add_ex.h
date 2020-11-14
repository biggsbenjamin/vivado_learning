int add_ex(int in_a, int in_b);

//image stream ex
#include "hls_stream.h"

typedef hls::stream<ap_axiu<16,1,1,1>>              AXI_STREAM;
typedef hls::Mat<MAX_HEIGHT, MAX_WIDTH, HLS_8UC2>   YUV_IMAGE;
typedef hls::Mat<MAX_HEIGHT, MAX_WIDTH, HLS_8UC3>   RGB_IMAGE;
