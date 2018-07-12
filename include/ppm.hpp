#ifndef PPM_H
#define PPM_H

#include <iostream>
#include <glm/glm.hpp>
#include <fstream>

typedef unsigned char uint8;

struct Pixel {
    Pixel(glm::vec3 v) {
        r = v.x * 255;
        g = v.y * 255;
        b = v.z * 255;
    }
    Pixel(){
        r = 0;
        g = 0;
        b = 0;
    }
    Pixel(uint8 _r, uint8 _g, uint8 _b) {
        r = _r;
        g = _g;
        b = _b;
    }
    uint8 r;
    uint8 g;
    uint8 b;
};

class PPM {
public:
    PPM(int w, int h){
        width = w;
        height = h;
        pixels = new Pixel[w * h];
    }
    ~PPM(){
        // delete pixels;
    }

    void setPixel(int y, int x, uint8 r, uint8 g, uint8 b) {
        pixels[y * width + x] = Pixel(r, g, b);
    }

    void setPixel(int y, int x, glm::vec3 v) {
        pixels[y * width + x] = Pixel(v);
    }

    void toFile(const char* filename) {
        std::ofstream f(filename);
        // std::cout << "Saving to: " << filepath << std::endl;
        f << "P3\n" << width << " " << height << "\n255\n";
        char buffer [13];
        for(int i = 0;i < width * height;i++) {
            sprintf(buffer, "%d %d %d\n", pixels[i].r, pixels[i].g, pixels[i].b);
            f << buffer;
        }
        f.close();
        // out_file.close();
        // std::ofstream file(filepath);
        // std::cout << "Writing Header..." << std::endl;
        // file << "P3\n" << width << " " << height << "\n255\n";
        // std::cout << "Size of Pixels: " << (sizeof(pixels) * sizeof(Pixel)) << std::endl;
        // char buffer [13];
        // for(int i = 0;i < width * height;i++ ) {
        //     std::cout << "(" << pixels[i].r << "," << pixels[i].g << "," << pixels[i].b << ")" << std::endl;
        //     sprintf(buffer, "%d %d %d\n", pixels[i].r, pixels[i].g, pixels[i].b);
        //     file << buffer;
        //     // file << pixels[i].r << " " << pixels[i].g << " " << pixels[i].b << "\n";
        // }
        // file.close();
    }
private:
    Pixel* pixels;
    int width, height;
};

#endif
