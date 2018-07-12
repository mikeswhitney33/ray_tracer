#ifndef TEXTURE_MATERIAL_H
#define TEXTURE_MATERIAL_H

#include <material.hpp>
#include <stb_image.h>

class TextureMaterial: public Material {
public:
    TextureMaterial(const char* filename, int n, float ka, float ks, float k_reflect=0.0f, float k_refract=0.0f, float i_refract=1.003):Material(n, ka, ks, k_reflect, k_refract, i_refract) {
        // img_data = stbi_load(filename, &width, &height, &nrChannels, 0);
        // stbi_image_free(img_data);


    }
    virtual ~TextureMaterial() {
        std::cout << "Destructing Texture Material" << std::endl;

    }

private:
    glm::vec3 getObjectColor(glm::vec3 pt) {
        return glm::vec3(0.0f, 0.0f, 0.0f);
        // glm::vec2 uv = shape->getUV(pt);
        //
        // int u = uv.x * width;
        // int v = uv.y * height;
        // glm::vec3 color;
        // color.x = img_data[(v * height + u) * nrChannels] / 255;
        // color.y = img_data[(v * height + u) * nrChannels + 1] / 255;
        // color.z = img_data[(v * height + u) * nrChannels + 2] / 255;
        // return color;
    }
    int width, height, nrChannels;
    unsigned char* img_data;

};

#endif
