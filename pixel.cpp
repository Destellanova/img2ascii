#include "pixel.h"

#include <cstring>

rgba_pixel* pixellate(unsigned char* raw_data, int w, int h, int c){
    rgba_pixel* array = new rgba_pixel[h*w];
    memcpy(array, raw_data, h*w*c);
    return array;
}

rgba_pixel* downsample(rgba_pixel* source, int w, int h, int scale){
    //Precompute some useful constants
    int SAMPLED_W = w/scale;
    int SAMPLED_H = h/(scale*2);
    int SAMPLED_LEN = SAMPLED_H*SAMPLED_W;
    int BOX_W = scale;
    int BOX_H = scale*2;


    //Initialise variables
    long r, g, b, a;
    rgba_pixel* sampled = new rgba_pixel[SAMPLED_LEN];

    for (int i=0; i<SAMPLED_LEN; i++){
        r = g = b = a = 0;
        int box_origin = (i%SAMPLED_W)*BOX_W + (i/SAMPLED_W)*w*BOX_H;
        for (int y=0; y<BOX_H; y++){
            for(int x=0; x<BOX_W; x++){
                int idx = box_origin+ x + (y*BOX_W*SAMPLED_W);
                r += source[idx].r;
                g += source[idx].g;
                b += source[idx].b;
                a += source[idx].a;
            }
        }
        int pixels = BOX_W*BOX_H;
        unsigned char rc = r/pixels; 
        unsigned char gc = g/pixels;
        unsigned char bc = b/pixels;
        unsigned char ac = a/pixels;
        sampled[i] = {rc, gc, bc, ac};
    }
    return sampled;
}

unsigned char brightness(rgba_pixel p){
    return p.a*(3*p.r + 10*p.g + p.b)/14/255;
}