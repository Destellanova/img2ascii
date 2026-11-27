
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
        rgba_pixarr pixel_array;
        pixel_array.data = pixellate(img_raw, width, height, channels);
        pixel_array.h = height;
        pixel_array.w = width;
        free(img_raw);

        printf("File %s loaded, with height %d, width %d, and %d channels, rescaled by %d\n", file_paths[path].data(), width, height, channels, scale);

        //Downsample pixel array
        downsample(&pixel_array, scale);
        
        //Print pixel_array as ascii
        pa_to_ascii(&pixel_array, BRIGHTNESS_MAP_DARK, 10);
                
    }


}