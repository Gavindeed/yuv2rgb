#ifndef YUV_H
#define YUV_H

class YUV
{
public:
    YUV(int h, int w);
    ~YUV();
    void Load(char *filename);
    void Append(char *filename);

    int height;
    int width;
    unsigned char *Y;
    unsigned char *U;
    unsigned char *V;
};

#endif //YUV_H
