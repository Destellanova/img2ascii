#pragma once

struct rgba_pixel{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
};
struct rgba_pixarr{
    rgba_pixel* data;
    int h;
    int w;
    ~rgba_pixarr();
};

rgba_pixel* pixellate(unsigned char* raw_data, int w, int h, int c);
void downsample(rgba_pixarr* pixarr, int scale);
unsigned char brightness(rgba_pixel p);
void pa_to_ascii(rgba_pixarr* pa, const char* map, int map_len);
