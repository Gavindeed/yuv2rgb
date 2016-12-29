#include <stdio.h>
#include "sse2converter.h"

unsigned char
sse2Converter::check(int num)
{
    if(num < 0) return 0;
    if(num > 255) return 255;
    return num;
}

void
sse2Converter::yuv2rgb(YUV *frame, RGB *group)
{
    int h = frame->height;
    int w = frame->width;
    /*for(int i = 0; i < h; i++)
    {
        for(int j = 0; j < w; j++)
        {
            
            group[i*w+j].R = check((298 * (frame->Y[i*w+j] - 16) + 409 * (frame->V[i/2*w/2+j/2] - 128)) >> 8);
            group[i*w+j].G = check((298 * (frame->Y[i*w+j] - 16) - 100 * (frame->U[i/2*w/2+j/2] - 128) - 208 * (frame->V[i/2*w/2+j/2] - 128)) >> 8);
            group[i*w+j].B = check((298 * (frame->Y[i*w+j] - 16) + 516 * (frame->U[i/2*w/2+j/2] - 128)) >> 8);
        }
    }*/

    for(int i=0;i<h;i+=1)
    {    
        for(int j=0;j<w;j+=8)
        {
            unsigned short c[8]={16,16,16,16,16,16,16,16};
            unsigned short d[8]={128,128,128,128,128,128,128,128};
            unsigned short e[8]={298,298,298,298,298,298,298,298};
            unsigned short f[8]={409,409,409,409,409,409,409,409};
            unsigned short z[8]={100,100,100,100,100,100,100,100};
            unsigned short h[8]={516,516,516,516,516,516,516,516};
            unsigned short x[8]={208,208,208,208,208,208,208,208};
            unsigned char y1,u1,v1,r1,g1,b1;unsigned char y2,u2,v2,r2,g2,b2;
            unsigned char y3,u3,v3,r3,g3,b3;unsigned char y4,u4,v4,r4,g4,b4;
            unsigned char y5,u5,v5,r5,g5,b5;unsigned char y6,u6,v6,r6,g6,b6;
            unsigned char y7,u7,v7,r7,g7,b7;unsigned char y8,u8,v8,r8,g8,b8;
            int iwj = i*w+j;
            y1 = frame->Y[iwj+0];y2 = frame->Y[iwj+1];
            y3 = frame->Y[iwj+2];y4 = frame->Y[iwj+3];
            y5 = frame->Y[iwj+4];y6 = frame->Y[iwj+5];
            y7 = frame->Y[iwj+6];y8 = frame->Y[iwj+7];
            u1 = frame->U[i/2*w/2+j/2];u2 = frame->U[i/2*w/2+(j+1)/2];
            u3 = frame->U[i/2*w/2+(j+2)/2];u4 = frame->U[i/2*w/2+(j+3)/2];
            u5 = frame->U[i/2*w/2+(j+4)/2];u6 = frame->U[i/2*w/2+(j+5)/2];
            v7 = frame->U[i/2*w/2+(j+6)/2];u8 = frame->U[i/2*w/2+(j+7)/2];
            v1 = frame->V[i/2*w/2+(j+0)/2];v2 = frame->V[i/2*w/2+(j+1)/2];
            v3 = frame->V[i/2*w/2+(j+2)/2];v4 = frame->V[i/2*w/2+(j+3)/2];
            v5 = frame->V[i/2*w/2+(j+4)/2];v6 = frame->V[i/2*w/2+(j+5)/2];
            v7 = frame->V[i/2*w/2+(j+6)/2];v8 = frame->V[i/2*w/2+(j+7)/2];
            unsigned short y[8]={y1,y2,y3,y4,y5,y6,y7,y8};
            unsigned short u[8]={u1,u2,u3,u4,u5,u6,u7,u8};
            unsigned short v[8]={v1,v2,v3,v4,v5,v6,v7,v8};
            unsigned short r[8]={0,0,0,0,0,0,0,0};
            unsigned short g[8]={0,0,0,0,0,0,0,0};
            unsigned short b[8]={0,0,0,0,0,0,0,0};
            // compute r by yuv
            asm
            (
            "movdqu (%0),%%xmm0;"  // y,u,v
            //"movdqu (%1),%%xmm1;"
            "movdqu (%2),%%xmm2;"
            "movdqu (%4),%%xmm4;" //16
            "movdqu (%5),%%xmm5;" //298
            "movdqu (%6),%%xmm6;" //409
            "movdqu (%7),%%xmm7;" //128
            "psubw %%xmm4,%%xmm0;" // y-16
            "pmullw %%xmm5,%%xmm0;" //298*(y-16)
            "psubw %%xmm7,%%xmm2;"  // v-128
            "pmullw %%xmm6,%%xmm2;" //409*(v-128)
            "paddd %%xmm2,%%xmm0;"  //+
            "psrlw $8,%%xmm0;" // >>8
            "movdqu %%xmm0,(%3);" //save
            "emms;"
            :
            :"p"(y),"p"(u),"p"(v),"p"(r),"p"(c),"p"(e),"p"(f),"p"(d)
            :
            );
            for(int k=0;k<8;k++)
                group[i*w+j+k].R = check(r[k]);
            asm
            (
            "movdqu (%0),%%xmm0;"  // y,u,v
            "movdqu (%1),%%xmm1;"
            "movdqu (%2),%%xmm2;" 
            "movdqu (%3),%%xmm3;" //298
            "movdqu (%4),%%xmm4;" //16
            "movdqu (%5),%%xmm5;" //100
            "movdqu (%6),%%xmm6;" //128
            "movdqu (%7),%%xmm7;" //208
            "psubw %%xmm4,%%xmm0;" // y-16
            "pmullw %%xmm3,%%xmm0;" // 298*(y-16)
            "psubw %%xmm6,%%xmm1;"  // u-128
            "pmullw %%xmm5,%%xmm1;" // 100*(u-128)
            "psubw %%xmm1,%%xmm0;"  // -
            "psubw %%xmm6,%%xmm2;"  // v-128
            "pmullw %%xmm7,%%xmm2;" // 208*(v-128)
            "psubw %%xmm2,%%xmm0;"  // -
            "psrlw $8,%%xmm0;"      // >>8
            "movdqu %%xmm0,(%8);"   // save it
            "emms;"
            :
            :"p"(y),"p"(u),"p"(v),"p"(e),"p"(c),"p"(z),"p"(d),"p"(x),"p"(g)
            :
            );
            for(int k=0;k<8;++k)
                group[i*w+j+k].G = check(g[k]);
            asm
            (
            "movdqu (%0),%%xmm0;"  // y,u,v
            "movdqu (%1),%%xmm1;"
            //"movdqu (%2),%%xmm2;" 
            "movdqu (%3),%%xmm3;" //298
            "movdqu (%4),%%xmm4;" //16
            "movdqu (%5),%%xmm5;" //516
            "movdqu (%6),%%xmm6;" //128
            "psubw %%xmm4,%%xmm0;" // y-16
            "pmullw %%xmm3,%%xmm0;" //298*(y-16)
            "psubw %%xmm6,%%xmm1;"  // u-128
            "pmullw %%xmm5,%%xmm1;" //409*(u-128)
            "paddd %%xmm1,%%xmm0;"  //+
            "psrlw $8,%%xmm0;" // >>8
            "movdqu %%xmm0,(%7);" //save
            "emms;"
            :
            :"p"(y),"p"(u),"p"(v),"p"(e),"p"(c),"p"(h),"p"(d),"p"(b)
            :    
            );
            for(int k=0;k<8;++k)
            group[i*w+j+k].B = check(b[k]);
        }
    }
}

void
sse2Converter::rgb2yuv(YUV *frame, RGB* group)
{
    int h = frame->height;
    int w = frame->width;
   /* for(int i = 0; i < h; i++)
    {
        for(int j = 0; j < w; j++)
        {
            frame->Y[i*w+j] = ((66 * group[i*w+j].R + 129 * group[i*w+j].G + 25 * group[i*w+j].B) >> 8) + 16;
            frame->U[i/2*w/2+j/2] = ((-38 * group[i*w+j].R - 74 * group[i*w+j].G + 112 * group[i*w+j].B) >> 8) + 128;
            frame->V[i/2*w/2+j/2] = ((112 * group[i*w+j].R - 94 * group[i*w+j].G - 18 * group[i*w+j].B) >> 8) + 128;
        }
    }*/
    for(int i =0;i<h;i+=1)
        for(int j=0;j<w;j+=8)
    {
        unsigned short a[8]={66,66,66,66,66,66,66,66};
        unsigned short c[8]={129,129,129,129,129,129,129,129};
        unsigned short d[8]={25,25,25,25,25,25,25,25};  
        unsigned short e[8]={16,16,16,16,16,16,16,16};
        unsigned short r[8],g[8],b[8];
        unsigned short y[8],u[8],v[8];
        for(int k=0;k<8;k++)
           {
            r[k] = group[i*w+j+k].R;
            g[k] = group[i*w+j+k].G;
            b[k] = group[i*w+j+k].B;
            y[k]=0,u[k]=0,v[k]=0;
           }   
        asm
        (   "movdqu (%0),%%xmm0;" // load r,g,b
            "movdqu (%1),%%xmm1;"
            "movdqu (%2),%%xmm2;"
            "movdqu (%3),%%xmm3;"//66
            "movdqu (%4),%%xmm4;"//129
            "movdqu (%5),%%xmm5;"//25
            "movdqu (%6),%%xmm6;"//16
            "pmullw %%xmm3,%%xmm0;"
            "pmullw %%xmm4,%%xmm1;"
            "pmullw %%xmm5,%%xmm3;"
            "paddd %%xmm1,%%xmm0;"
            "paddd %%xmm2,%%xmm0;"
            "psrlw $8,%%xmm0;"
            "paddd %%xmm6,%%xmm0;"
            "movdqu %%xmm0,(%7);"
            "emms;"
            :
            :"p"(r),"p"(g),"p"(b),"p"(a),"p"(c),"p"(d),"p"(e),"p"(y)
            :
        );
            
        unsigned short f[8]={38,38,38,38,38,38,38,38};
        unsigned short h[8]={74,74,74,74,74,74,74,74};
        unsigned short p[8]={112,112,112,112,112,112,112,112};
        unsigned short t[8]={128,128,128,128,128,128,128,128};
        asm
        (
            "movdqu (%0),%%xmm0;" //b
            "movdqu (%1),%%xmm1;" //r
            "movdqu (%2),%%xmm2;"//g
            "movdqu (%3),%%xmm3;"//112
            "movdqu (%4),%%xmm4;"//38
            "movdqu (%5),%%xmm5;"//74
            "movdqu (%6),%%xmm6;"//128
            "pmullw %%xmm3,%%xmm0;"
            "pmullw %%xmm4,%%xmm1;"
            "pmullw %%xmm5,%%xmm2;"
            "psubw %%xmm1,%%xmm0;"
            "psubw %%xmm2,%%xmm0;"
            "psrlw $8,%%xmm0;"
            "paddd %%xmm6,%%xmm0;"
            "movdqu %%xmm0,(%7);"
            "emms;"
            :
            :"p"(b),"p"(r),"p"(g),"p"(p),"p"(f),"p"(h),"p"(t),"p"(u)
            :
        );
        unsigned short l[8]={94,94,94,94,94,94,94,94};
        unsigned short m[8]={18,18,18,18,18,18,18,18};
        asm
        (
            "movdqu (%0),%%xmm0;" //r
            "movdqu (%1),%%xmm1;" //g
            "movdqu (%2),%%xmm2;"//b
            "movdqu (%3),%%xmm3;"//112
            "movdqu (%4),%%xmm4;"//94
            "movdqu (%5),%%xmm5;"//18
            "movdqu (%6),%%xmm6;"//128
            "pmullw %%xmm3,%%xmm0;"
            "pmullw %%xmm4,%%xmm1;"
            "pmullw %%xmm5,%%xmm2;"
            "psubw %%xmm1,%%xmm0;"
            "psubw %%xmm2,%%xmm0;"
            "psrlw $8,%%xmm0;"
            "paddd %%xmm7,%%xmm0;"
            "movdqu %%xmm0,(%7);"
            "emms;"
            :
            :"p"(r),"p"(g),"p"(b),"p"(p),"p"(l),"p"(m),"p"(t),"p"(v)
            :
        );
        int iw = i*w;
        int i2w = i/2*w/2;
        for(int k=0;k<8;++k)
            {
                frame->Y[iw+j+k] = check(y[k]);
                frame->V[i2w+(j+k)>>1] = check(v[k]);
                frame->U[i2w+(j+k)>>1] = check(u[k]);
               // frame->Y[i*w+j+k] = ((66 * group[i*w+j+k].R + 129 * group[i*w+j+k].G + 25 * group[i*w+j+k].B) >> 8) + 16;
               // frame->U[i/2*w/2+(j+k)/2] = ((-38 * group[i*w+j+k].R - 74 * group[i*w+j+k].G + 112 * group[i*w+j+k].B) >> 8) + 128;
               // frame->V[i/2*w/2+(j+k)/2] = ((112 * group[i*w+j+k].R - 94 * group[i*w+j+k].G - 18 * group[i*w+j+k].B) >> 8) + 128;
            }    
        
    }       
}

void 
sse2Converter::alpha(RGB *group, RGB *out, unsigned char alpha, int size)
{
    for(int i = 0; i < size; i+=8)
    {
        unsigned short a[8];
        unsigned short r[8],g[8],b[8];
        unsigned short r2[8],g2[8],b2[8];
        for(int k=0;k<8;k++)
        {
            int temp = i+k;
            r[k] = group[temp].R;
            g[k] = group[temp].G;
            b[k] = group[temp].B;
            a[k] = alpha;
            r2[k]=0,g2[k]=0,b2[k]=0;
        }
        asm
        (
            "movdqu (%0),%%xmm0;"
            "movdqu (%2),%%xmm1;"
            "pmullw %%xmm1,%%xmm0;"
            "psrlw $8,%%xmm0;"
            "movdqu %%xmm0,(%1);"
            "movdqu (%3),%%xmm2;"
            "pmullw %%xmm1,%%xmm2;"
            "psrlw $8,%%xmm2;"
            "movdqu %%xmm2,(%4);"
            "movdqu (%5),%%xmm4;"
            "pmullw %%xmm1,%%xmm4;"
            "psrlw $8,%%xmm4;"
            "movdqu %%xmm4,(%6);"
            :
            :"p"(r),"p"(r2),"p"(a),"p"(g),"p"(g2),"p"(b),"p"(b2)
            :
        );
        for(int k=0;k<8;k++)
        {
            int temp = i+k;
            out[temp].R = r2[k];
            out[temp].G = g2[k];
            out[temp].B = b2[k]; 
        }
        /*out[i].R = group[i].R * alpha / 256;
        out[i].G = group[i].G * alpha / 256;
        out[i].B = group[i].B * alpha / 256;*/
    }
}

void
sse2Converter::merge(RGB *group1, RGB *group2, RGB *out, unsigned char alpha, int size)
{
    for(int i = 0; i < size; i+=8)
    {
        unsigned short r1[8],g1[8],b1[8];
        unsigned short r2[8],g2[8],b2[8];
        unsigned short r3[8],g3[8],b3[8];
        unsigned short a1[8],a2[8];
        for(int k=0;k<8;k++)
        {
            r1[k]=group1[i].R,r2[k]= group2[i].R;
            g1[k]=group1[i].G,g2[k]= group2[i].G;
            b1[k]=group1[i].B,b2[k]= group2[i].B;
            r3[k]=0,g3[k]=0,b3[k]=0;
            a1[k]=alpha,a2[k]=256-alpha;
        }
        asm
        (
            "movdqu (%0),%%xmm0;"
            "movdqu (%1),%%xmm1;"
            "movdqu (%2),%%xmm2;"
            "movdqu (%3),%%xmm3;"
            "pmullw %%xmm2,%%xmm0;"
            "pmullw %%xmm3,%%xmm1;"
            "paddd %%xmm1,%%xmm0;"
            "psrlw $8,%%xmm0;"
            "movdqu %%xmm0,(%4);"
            :
            :"p"(r1),"p"(r2),"p"(a1),"p"(a2),"p"(r3)
            :
        );
         asm
        (
            "movdqu (%0),%%xmm0;"
            "movdqu (%1),%%xmm1;"
            "movdqu (%2),%%xmm2;"
            "movdqu (%3),%%xmm3;"
            "pmullw %%xmm2,%%xmm0;"
            "pmullw %%xmm3,%%xmm1;"
            "paddd %%xmm1,%%xmm0;"
            "psrlw $8,%%xmm0;"
            "movdqu %%xmm0,(%4);"
            :
            :"p"(g1),"p"(g2),"p"(a1),"p"(a2),"p"(g3)
            :
        );
         asm
        (
            "movdqu (%0),%%xmm0;"
            "movdqu (%1),%%xmm1;"
            "movdqu (%2),%%xmm2;"
            "movdqu (%3),%%xmm3;"
            "pmullw %%xmm2,%%xmm0;"
            "pmullw %%xmm3,%%xmm1;"
            "paddd %%xmm1,%%xmm0;"
            "psrlw $8,%%xmm0;"
            "movdqu %%xmm0,(%4);"
            :
            :"p"(b1),"p"(b2),"p"(a1),"p"(a2),"p"(b3)
            :
        );
        for(int k=0;k<8;k++)
        {
          int temp = i+k;
          out[temp].R=r3[k];
          out[temp].G=g3[k];
          out[temp].B=b3[k]; 
        }
        /*out[i].R = (alpha * group1[i].R + (256 - alpha) * group2[i].R) / 256;
        out[i].G = (alpha * group1[i].G + (256 - alpha) * group2[i].G) / 256;
        out[i].B = (alpha * group1[i].B + (256 - alpha) * group2[i].B) / 256;
        */    
    }
}
