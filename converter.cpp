#include <stdio.h>
#include "converter.h"

unsigned char
Converter::check(int num)
{
    if(num < 0) return 0;
    if(num > 255) return 255;
    return num;
}

void
Converter::yuv2rgb(YUV *frame, RGB *group)
{
    int h = frame->height;
    int w = frame->width;
    for(int i = 0; i < h; i++)
    {
        for(int j = 0; j < w; j++)
        {
            //printf("%d %d\n", i, j);
            group[i*w+j].R = check(1.164383 * (frame->Y[i*w+j] - 16) + 1.596027 * (frame->V[i/2*w/2+j/2] - 128));
            group[i*w+j].G = check(1.164383 * (frame->Y[i*w+j] - 16) - 0.391762 * (frame->U[i/2*w/2+j/2] - 128) - 0.812968 * (frame->V[i/2*w/2+j/2] - 128));
            group[i*w+j].B = check(1.164383 * (frame->Y[i*w+j] - 16) + 2.017232 * (frame->U[i/2*w/2+j/2] - 128));
        }
    }
}

void
Converter::rgb2yuv(YUV *frame, RGB* group)
{
    int h = frame->height;
    int w = frame->width;
    for(int i = 0; i < h; i++)
    {
        for(int j = 0; j < w; j++)
        {
            frame->Y[i*w+j] = 0.256788 * group[i*w+j].R + 0.504129 * group[i*w+j].G + 0.097906 * group[i*w+j].B + 16;
            frame->U[i/2*w/2+j/2] = -0.148233 * group[i*w+j].R - 0.290933 * group[i*w+j].G + 0.439216 * group[i*w+j].B + 128;
            frame->V[i/2*w/2+j/2] = 0.439216 * group[i*w+j].R -0.367788 * group[i*w+j].G - 0.071427 * group[i*w+j].B + 128;
        }
    }
}

void 
Converter::alpha(RGB *group, RGB *out, unsigned char alpha, int size)
{
    for(int i = 0; i < size; i++)
    {
        out[i].R = group[i].R * alpha / 256;
        out[i].G = group[i].G * alpha / 256;
        out[i].B = group[i].B * alpha / 256;
    }
}

void
Converter::merge(RGB *group1, RGB *group2, RGB *out, unsigned char alpha, int size)
{
    for(int i = 0; i < size; i++)
    {
        out[i].R = (alpha * group1[i].R + (256 - alpha) * group2[i].R) / 256;
        out[i].G = (alpha * group1[i].G + (256 - alpha) * group2[i].G) / 256;
        out[i].B = (alpha * group1[i].B + (256 - alpha) * group2[i].B) / 256;
    }
}
