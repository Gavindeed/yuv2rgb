#include <stdlib.h>
#include <stdio.h>
#include "yuv.h"

YUV::YUV(int h, int w)
{
    height = h;
    width = w;
    Y = (unsigned char*)malloc(h*w);
    U = (unsigned char*)malloc(h*w/4);
    V = (unsigned char*)malloc(h*w/4);
}

void YUV::Load(char *filename)
{
    FILE *infile;
    infile = fopen(filename, "rb");
    fread(Y, 1, height*width, infile);
    fread(U, 1, height*width/4, infile);
    fread(V, 1, height*width/4, infile);
    fclose(infile);
}

void YUV::Append(char *filename)
{
    FILE *output;
    static int sum = 0;
    static int times = 0;
    output = fopen(filename, "ab");
    sum += fwrite(Y, 1, height*width, output);
    sum += fwrite(U, 1, height*width/4, output);
    sum += fwrite(V, 1, height*width/4, output);
    printf("write %d %d\n", ++times, sum);
    fclose(output);
}

YUV::~YUV()
{
    free(Y);
    free(U);
    free(V);
}
