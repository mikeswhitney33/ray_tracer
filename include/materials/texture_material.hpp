#ifndef TEXTURE_MATERIAL_H
#define TEXTURE_MATERIAL_H

#include <material.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>


class TextureMaterial: public Material {
public:
    TextureMaterial(const char* filename, const int &n, const float &ka, const float &ks, float k_reflect=0.0f, float k_refract=0.0f, float i_refract=1.003):Material(n, ka, ks, k_reflect, k_refract, i_refract) {
        cv::Mat img = cv::imread(filename, cv::IMREAD_COLOR);
        width = img.cols;
        height = img.rows;
        nrChannels = img.channels();
        img_data = new unsigned char[width * height * nrChannels];
        memcpy(img_data, img.data, width * height * nrChannels);
        id = 1;
        img.release();
    }

    TextureMaterial(Material* mat): Material(mat) {
        TextureMaterial* t_mat = dynamic_cast<TextureMaterial*>(mat);
        width = t_mat->width;
        height = t_mat->height;
        nrChannels = t_mat->nrChannels;
        img_data = new unsigned char[width * height * nrChannels];
        memcpy(img_data, t_mat->img_data, width * height * nrChannels);
        id = t_mat->id;
    }
    virtual ~TextureMaterial() {
        delete img_data;
    }

    glm::vec3 getObjectColor(const glm::vec2 &uv) {
        int u = (int)(uv.x * width) % width;
        int v = (int)(uv.y * height) % height;
        int idx = (v * height + u) * nrChannels;

        glm::vec3 color;
        color.x = img_data[idx + (2 % nrChannels)] / 255.0f;
        color.y = img_data[idx + (1 % nrChannels)] / 255.0f;
        color.z = img_data[idx] / 255;
        return color;
    }

    virtual Material* copy() {
        return new TextureMaterial(this);
    }

private:

    int width, height, nrChannels;
    unsigned char* img_data;
};

#endif
