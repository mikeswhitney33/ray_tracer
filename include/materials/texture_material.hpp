#ifndef TEXTURE_MATERIAL_H
#define TEXTURE_MATERIAL_H

#include <material.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
// #include <stb_image.h>
// #include <FreeImage.h>


class TextureMaterial: public Material {
public:
    TextureMaterial(const char* filename, int n, float ka, float ks, float k_reflect=0.0f, float k_refract=0.0f, float i_refract=1.003):Material(n, ka, ks, k_reflect, k_refract, i_refract) {
        // img_data = stbi_load(filename, &width, &height, &nrChannels, 0);
        // stbi_image_free(img_data);
        // FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(filename,0);//Automatocally detects the format(from over 20 formats!)
        // img_data = cv::imread(filename, cv::IMREAD_COLOR).data;
        cv::Mat img = cv::imread(filename, cv::IMREAD_COLOR);
        width = img.cols;
        height = img.rows;
        nrChannels = img.channels();
        img_data = new unsigned char[width * height * nrChannels];
        memcpy(img_data, img.data, width * height * nrChannels);

    }
    virtual ~TextureMaterial() {
        delete img_data;
    }

private:
    glm::vec3 getObjectColor(glm::vec3 pt) {
        // return glm::vec3(0.0f, 0.0f, 0.0f);
        // for(int i = 0;i < width * height * nrChannels;i+=3) {
        //     std::cout << "(" << (int)img_data[i + 2] << "," << (int)img_data[i + 1] << "," << (int)img_data[i] << ")" << std::endl;
        // }

        glm::vec2 uv = shape->getUV(pt);


        int u = (int)(uv.x * width) % width;
        int v = (int)(uv.y * height) % height;
        int idx = (v * height + u) * nrChannels;


        glm::vec3 color;
        color.x = img_data[idx + (2 % nrChannels)] / 255.0f;
        color.y = img_data[idx + (1 % nrChannels)] / 255.0f;
        color.z = img_data[idx] / 255;
        return color;
    }
    int width, height, nrChannels;
    unsigned char* img_data;

};

#endif
