#ifndef PPM_H
#define PPM_H

#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include <cstdlib>

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
        delete pixels;
    }

    void setPixel(int y, int x, uint8 r, uint8 g, uint8 b) {
        pixels[y * width + x] = Pixel(r, g, b);
    }

    void setPixel(int y, int x, glm::vec3 v) {
        pixels[y * width + x] = Pixel(v);
    }

    void toFile(const char* filepath){
        std::ofstream file;
        file.open(filepath);
        file << "P3\n" << width << " " << height << "\n255\n";
        char buffer [13];
        for(int i = 0;i < width * height;i++ ) {
            sprintf(buffer, "%d %d %d\n", pixels[i].r, pixels[i].g, pixels[i].b);
            file << buffer;
            // file << pixels[i].r << " " << pixels[i].g << " " << pixels[i].b << "\n";
        }
    }
private:
    Pixel* pixels;
    int width, height;
};

#endif
