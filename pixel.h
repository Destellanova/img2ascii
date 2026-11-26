#pragma once

struct rgba_pixel{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
};

rgba_pixel* pixellate(unsigned char* raw_data, int w, int h, int c);
rgba_pixel* downsample(rgba_pixel* source, int w, int h, int scale);
unsigned char brightness(rgba_pixel p);
