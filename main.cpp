#include <iostream>
#include <time.h>
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

double process_without_simd_1();
double process_without_simd_2();

int main(){
    converter = new Converter();
    cout << process_without_simd_1() << endl;
    cout << process_without_simd_2() << endl;
	return 0;
}



double process_without_simd_1(){
	double t = 0;
    double start, end;

    frame = new YUV(1080, 1920);
    frame->Load("dem1.yuv");
    group1 = new RGB[1080*1920];
    out = new RGB[1080*1920];
    start = clock();
    converter->yuv2rgb(frame, group1);
    for(unsigned char a = 0; a < 255; a += 3)
    {
        converter->alpha(group1, out, a, 1080*1920);
        converter->rgb2yuv(frame, out);
        frame->Append("out1.yuv");
    }
    end = clock();
    t = (end - start) / CLOCKS_PER_SEC;
    delete frame;
    delete [] group1;
    delete [] out;

	return t;
}

double process_without_simd_2() {
    double t = 0;
    double start, end;

    frame = new YUV(1080, 1920);
    frame->Load("dem1.yuv");
    group1 = new RGB[1080*1920];
    converter->yuv2rgb(frame, group1);
    frame->Load("dem2.yuv");
    group2 = new RGB[1080*1920];
    converter->yuv2rgb(frame, group2);
    out = new RGB[1080*1920];
    start = clock();
    for(unsigned char a = 0; a < 255; a += 3)
    {
        converter->merge(group1, group2, out, a, 1080*1920);
        converter->rgb2yuv(frame, out);
        frame->Append("out2.yuv");
    }
    end = clock();
    t = (end - start) / CLOCKS_PER_SEC;
    delete frame;
    delete [] group1;
    delete [] group2;
    delete [] out;

    return t;
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
