#include <iostream>
#include "rgb.h"
#include "yuv.h"
#include "converter.h"

using namespace std;

Converter *converter;
RGB *group1, *group2;
RGB *out;
YUV *frame;
FILE *output;
//RGB out[84][1024][1920];

int process_without_simd_1();
int process_without_simd_2();

int main(){
    converter = new Converter();
    //process_without_simd_1();
    process_without_simd_2();
	return 0;
}



int process_without_simd_1(){
	int time = 0;

    frame = new YUV(1080, 1920);
    frame->Load("dem1.yuv");
    group1 = new RGB[1080*1920];
    out = new RGB[1080*1920];
    converter->yuv2rgb(frame, group1);
    for(unsigned char a = 0; a < 255; a += 3)
    {
        converter->alpha(group1, out, a, 1080*1920);
        converter->rgb2yuv(frame, out);
        frame->Append("out1.yuv");
    }
    delete frame;
    delete [] group1;
    delete [] out;

	return time;
}

int process_without_simd_2() {
    int time = 0;

    frame = new YUV(1080, 1920);
    frame->Load("dem1.yuv");
    group1 = new RGB[1080*1920];
    converter->yuv2rgb(frame, group1);
    frame->Load("dem2.yuv");
    group2 = new RGB[1080*1920];
    converter->yuv2rgb(frame, group2);
    out = new RGB[1080*1920];
    for(unsigned char a = 0; a < 255; a += 3)
    {
        converter->merge(group1, group2, out, a, 1080*1920);
        converter->rgb2yuv(frame, out);
        frame->Append("out2.yuv");
    }
    delete frame;
    delete [] group1;
    delete [] group2;
    delete [] out;

    return time;
}

int process_with_mmx(){
	int time = 0;


	return time;
}

int process_with_sse(){
	int time = 0;


	return time;
}

int process_with_avx(){
	int time = 0;


	return time;
}
