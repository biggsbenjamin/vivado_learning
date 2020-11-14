#include "add_ex.h"

/*int add_ex(int in_a, int in_b)
{
#pragma HLS INTERFACE s_axilite port=in_b

    int res;

    res = in_a + in_b;

    return res;
}*/

void image_filter(AXI_STREAM& video_in, AXI_STREAM& video_out, int rows, int cols) {
    //create the streaming interfaces
#pragma HLS INTERFACE axis port=video_in bundle=INPUT_STREAM
#pragma HLS INTERFACE axis port=video_out bundle=OUTPUT_STREAM

#pragma HLS INTERFACE s_axilite port=rows bundle=CONTROL_BUS offset=0x14
#pragma HLS INTERFACE s_axilite port=cols bundle=CONTROL_BUS offset=0x1C
#pragma HLS INTERFACE s_axilite port=return bundle=CONTROL_BUS

#pragma HLS INTERFACE ap_stable port=rows
#pragma HLS INTERFACE ap_stable port=cols

    YUV_IMAGE img_0(rows, cols);
    YUV_IMAGE img_1(rows, cols);

#pragma HLS dataflow
    hls::AXIvideo2Mat(video_in, img_0);
    hls::GaussianBlur<3,3>(img_0, img_1, 0, 0);
    hls::Mat2AXIvideo(img_1, video_out);
}
