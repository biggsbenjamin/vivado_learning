#include <stdio.h>
#include "add_ex.h"

/*int main()
{
    int i;
    int result;
    int rand_a, rand_b;
    int tb_result;

    for(i=0;i<10;i++){
        rand_a = rand();
        rand_b = rand();
        tb_result = rand_a + rand_b;
        result = add_ex(rand_a, rand_b);
        if (tb_result != result ){
            printf("mismatch %d %d \n",tb_result, result);

        }
        else{
            printf("Success %d %d \n",tb_result, result);
        }
    }
    printf("simulation complete\n");
    return 0;
}*/


int main (int argc, char** argv){
    Mat src_rgb = imread(INPUT_IMAGE);
    if (!src_rgb.data) {
        printf("error no image\n");
        return -1;
    }
    Mat src_yuv(src_rgb.rows, src_rgb.cols, CV-8UC2);
    Mat dst_yuv(src_rgb.rows, src_rgb.cols, CV-8UC2);
    Mat dst_rgb(src_rgb.rows, src_rgb.cols, CV-8UC3);

    cvtcolor_rgb2yuv422(src_rgb, src_yuv);
    
    IplImage src = src_yuv;
    IplImage dst = dst_yuv;
    hls_image_filter(&src, &dst);
    cvtColor(dst_yuv, dst_rgb, CV_YUV2BGR_YUYV);
    imwrite(OUTPUT_IMAGE, dst_rgb);
}

void hls_image_filter(IplImage *src, IplImage *dst) {
    AXI_STREAM src_axit, dst_axi;
    IplImage2AXIvideo(src, src_axi);
    image_filter(src_axi, dst_axi, src->height, src->width);
    AXIvideo2IplImage(dst_axi, dst);
}
