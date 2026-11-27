#include "pixel.h"

#include <cstring>
#include <stdlib.h>
#include <cstdio>


rgba_pixarr::~rgba_pixarr(){
    free(data);
}

rgba_pixel* pixellate(unsigned char* raw_data, int w, int h, int c){
    rgba_pixel* array = new rgba_pixel[h*w];
    memcpy(array, raw_data, h*w*c);
    return array;
}

void downsample(rgba_pixarr* pixarr, int scale){
    //Precompute some useful constants
    int SAMPLED_W = pixarr->w/scale;
    int SAMPLED_H = pixarr->h/(scale*2);
    int SAMPLED_LEN = SAMPLED_H*SAMPLED_W;
    int BOX_W = scale;
    int BOX_H = scale*2;

    //Initialise variables
    long r, g, b, a;
    rgba_pixel* sampled = new rgba_pixel[SAMPLED_LEN];
    rgba_pixel* source = pixarr->data;

    for (int i=0; i<SAMPLED_LEN; i++){
        r = g = b = a = 0;
        int box_origin = (i%SAMPLED_W)*BOX_W + (i/SAMPLED_W)*pixarr->w*BOX_H;
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
    
    free(source);
    pixarr->data = sampled;
    pixarr->h = SAMPLED_H;
    pixarr->w = SAMPLED_W;
    return;
}

unsigned char brightness(rgba_pixel p){
    return p.a*(3*p.r + 10*p.g + p.b)/14/255;
}

void pa_to_ascii(rgba_pixarr* pa, const char* map, int map_len){
    for(int i=0; i<pa->h; i++){
        for(int j=0; j<pa->w; j++){
            printf("%c", map[brightness(pa->data[pa->w*i + j]) *(map_len-1)/255]);
        }
        printf("\n");
    }
    return;
}