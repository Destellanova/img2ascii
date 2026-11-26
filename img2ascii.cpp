
#include <vector>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include <algorithm>
#include "pixel.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Argument flags
bool OUTPUT_AS_FILE = false;
bool STYLE_COLOUR = false;

// Other stuff

const char* BRIGHTNESS_MAP_DARK = "@%#*+=-:. ";
const char* BRIGHTNESS_MAP_LIGHT = " .:-=+*#%@";

int get_terminal_width(){
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0 && ws.ws_col > 0) {
        return ws.ws_col;
    }
    return 0;
}

int main(int argc, char* argv[]){
    // Parse arguments
    if (argc == 1) {
        printf("Too few arguments, exiting...\n");
        return -1;
    }

    std::vector<std::string> args(argv+1, argv+argc);

    std::vector<std::string> file_paths;
    for (int i=0; i<args.size(); i++){
        if (args[i]=="-o"){ //Output
            OUTPUT_AS_FILE = true;
        } else if (args[i]=="-s"){ //Style
            STYLE_COLOUR = true;
        } else if (args[i]=="-b"){ //Batch: get all images in folder

        } else { //Input file(s)
            file_paths.push_back(args[i]);
        }
    }
    // ----Flags set and a list of input file paths----
    
    // Execute on each file
    for (int path=0; path<file_paths.size(); path++){
        int width, height, channels;
        if (!stbi_info(file_paths[path].data(), &width, &height, &channels)){ //Check if format is supported
            printf("File %s is invalid, skipping...\n", file_paths[path].data());
            continue;
        }

        //Valid file
        int scale = std::max(1, (width/get_terminal_width())*2);

        //Process file into a pixel array
        unsigned char* img_raw = stbi_load(file_paths[path].data(), &width, &height, &channels, 0);
        printf("File %s loaded, with height %d, width %d, and %d channels, rescaled by %d\n", file_paths[path].data(), width, height, channels, scale);
        rgba_pixel* pixel_array = pixellate(img_raw, width, height, channels);
        free(img_raw);

        //Downsample pixel array
        pixel_array = downsample(pixel_array, width, height, scale);
        int pixarr_w = width/scale;
        int pixarr_h = height/scale/2;
        for(int i=0; i<pixarr_h; i++){
            for(int j=0; j<pixarr_w; j++){
                printf("%c", BRIGHTNESS_MAP_LIGHT[brightness(pixel_array[pixarr_w*i+j])*9/255]);
            }
            printf("\n");
        }
        free(pixel_array);
                
    }
    // Free memory


}