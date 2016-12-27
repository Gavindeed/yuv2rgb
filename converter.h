#ifndef CONVERTER_H
#define CONVERTER_H

#include "yuv.h"
#include "rgb.h"

class Converter
{
public:
    void yuv2rgb(YUV *frame, RGB *group);
    void rgb2yuv(YUV *frame, RGB *group);
    unsigned char check(int num);
    void alpha(RGB *group, RGB *out, unsigned char alpha, int size);
    void merge(RGB *group1, RGB *group2, RGB *out, unsigned char alpha, int size);
};

#endif //CONVERTER_H
