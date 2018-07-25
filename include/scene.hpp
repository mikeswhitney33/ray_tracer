#ifndef SCENE_H
#define SCENE_H

// #include <fstream>
#include <thread>
#include <vector>

#include <glm/glm.hpp>
#include <geometry.hpp>
#include <camera.hpp>
#include <light.hpp>

#include <material.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <model.hpp>

#include <timing.hpp>

#include <bounding_box.hpp>
#include <ray.hpp>
#include <octree.hpp>

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <utils.hpp>


#define SMALL_NUM ((float)pow(2, -13))


class Scene {
public:
    Scene(const glm::vec3 &bkgd, const Camera &cam, const int &w, const int &h, const float &fov, const int &recursions, const float &eta) {
        backgroundColor = bkgd;
        camera = cam;
        width = w;
        height = h;
        float fov_x, fov_y;
        fov_x = fov;
        fov_y = fov * ((w > h) ? (float)h / (float)w : (float) w / (float)h);

        u_max = 2 * (float) tan(glm::radians(fov_x / 2.0f));
        v_max = 2 * (float) tan(glm::radians(fov_y / 2.0f));
        max_recursions = recursions;
        initial_eta = eta;

        tree = new Octree(BoundingBox(glm::vec3(-999.0f, -999.0f, -999.0f), glm::vec3(999.0f, 999.0f, 999.0f)));
    }

    virtual ~Scene() {
        // for(int i = 0;i < lights.size();i++) {
        //     delete lights[i];
        // }
        delete tree;
    }

    void add_shape(Geometry* shape) {
        tree->add_shape(shape);
        if(shape->isLight()) {
            lights.push_back(static_cast<Light*>(shape));
        }
    }


    void add_model(const Model &model) {
        std::vector<Geometry*> s = model.getShapes();
        for(int i = 0;i < s.size();i++) {
            add_shape(s[i]);
        }
    }

    // void add_light( Light* light) {
    //     lights.push_back(light);
    // }

    static void set_color(Scene* context, int y, int x, cv::Mat m) {
        glm::vec3 color = context->getColor(y, x);
        float mx = glm::compMax(color);
        float mn = glm::compMin(color);
        color = (color - mn) / (mx - mn);
        color *= 255;
        m.at<cv::Vec3f>(y, x).val[2] = color.x;
        m.at<cv::Vec3f>(y, x).val[1] = color.y;
        m.at<cv::Vec3f>(y, x).val[0] = color.z;
    }

    void save(const char* filename) {
        cv::Mat m;
        m.create(height, width, CV_32FC(3));

        ProgressBar bar(height);
        std::thread workers[width];
        for(int y = 0;y < height;y++) {
            for(int x = 0;x < width;x++) {
                workers[x] = std::thread(set_color, this, y, x, m);
            }
            for(int x = 0; x< width;x++) {
                workers[x].join();
            }
            bar.tick();
        }

        cv::normalize(m, m, 0, 255, cv::NORM_MINMAX, CV_32FC3);

        std::vector<int> params;
        params.push_back(cv::IMWRITE_PNG_COMPRESSION);
        params.push_back(9);

        cv::imwrite(filename, m);
        std::cout << std::endl;
        m.release();
    }

protected:

    std::vector<Light*> lights;
    glm::vec3 backgroundColor;
    Camera camera;
    int width, height;
    float u_max, v_max;
    int max_recursions;
    float initial_eta;

    glm::vec3 uniform_sample_hemisphere(const float &r1, const float &r2) {
        float sinTheta = sqrtf(1 - r1 * r2);
        float phi = 2 * M_PI * r2;
        float x = sinTheta * cosf(phi);
        float z = sinTheta * sinf(phi);
        return glm::vec3(x, r1, z);
    }

    virtual glm::vec3 diffuse(const glm::vec3 &pt, glm::vec3 rd, const glm::vec3 &normal, const int  &recursions, const float &eta1, const glm::vec3 &Nt, const glm::vec3 &Nb, const float &r1, const float &r2) {
        glm::vec3 sam = uniform_sample_hemisphere(r1, r2);

        rd = glm::vec3(
            sam.x * Nb.x + sam.y * normal.x + sam.z * Nt.x,
            sam.x * Nb.y + sam.y * normal.y + sam.z * Nt.y,
            sam.x * Nb.z + sam.y * normal.z + sam.z * Nt.z);
        return sample(Ray(DIFFUSE, pt + rd * SMALL_NUM, rd), recursions + 1, eta1);
    }

    virtual glm::vec3 reflect(const glm::vec3 &pt, glm::vec3 rd, const glm::vec3 &normal, const int &recursions, const float &eta1) {
        rd = glm::reflect(rd, normal);
        return sample(Ray(REFLECT, pt + rd * SMALL_NUM, rd), recursions + 1, eta1);
    }

    virtual glm::vec3 refract(const glm::vec3 &pt, glm::vec3 rd, glm::vec3 normal, float eta1, float eta2, const int &recursions) {
        float ndoti = glm::dot(rd, normal);
        if(ndoti < 0) {
            ndoti = -ndoti;
        } else {
            normal = -normal;
            std::swap(eta1, eta2);
        }
        float eta = eta1 / eta2;
        rd = glm::refract(rd, normal, eta);
        return sample(Ray(REFRACT, pt + rd * SMALL_NUM, rd), recursions + 1, eta2);
    }

    virtual std::vector<bool> inShadow(glm::vec3 r0) {
        std::vector<bool> shadows;
        for(int i = 0;i < lights.size();i++) {
            float light_dist = lights[i]->getDistance(r0);
            glm::vec3 rd = glm::normalize(lights[i]->getDirection(r0));
            float t;
            glm::vec3 n;
            glm::vec2 uv;
            Geometry* shape;
            if(intersect(Ray(r0 + SMALL_NUM * rd, rd), t, n, uv, shape)){
                glm::vec3 inter = r0 + t * rd;
                float dist = glm::distance(r0, inter);
                if(dist < light_dist) {
                    shadows.push_back(true);
                    continue;
                }
            }
            shadows.push_back(false);
        }
        return shadows;
    }

    virtual bool intersect(const Ray &ray, float &t, glm::vec3 &normal, glm::vec2 &uv, Geometry* &shape) {
        return tree->intersect(ray, normal, t, uv, shape);
    }

    virtual glm::vec3 illuminate(const Material* &mat, const glm::vec3 &interPos, const glm::vec3 &normal, const glm::vec3 &viewPos, const glm::vec2 &uv, const std::vector<bool> &shadows) {
        glm::vec3 objectColor = mat->getObjectColor(interPos);
        glm::vec3 final_light = glm::vec3(0.0f, 0.0f, 0.0f);
        for(int i = 0;i < lights.size();i++) {
            if(!shadows[i]) {
                Light* light = lights[i];
                glm::vec3 lightColor = light->getColor();


                glm::vec3 ambient = mat->ambientStrength * lightColor;

                glm::vec3 norm = glm::normalize(normal);
                glm::vec3 lightDir = glm::normalize(light->getDirection(interPos));
                float diff = std::max(glm::dot(norm, lightDir), 0.0f);
                glm::vec3 diffuse = diff * lightColor;

                glm::vec3 viewDir = glm::normalize(viewPos - interPos);
                glm::vec3 reflectDir = glm::reflect(-lightDir, norm);
                float spec = pow(std::max(glm::dot(viewDir, reflectDir), 0.0f), mat->phong);
                glm::vec3 specular = mat->specularStrength * spec * lightColor;

                final_light += (ambient + diffuse + specular);
            }
        }
        return final_light * objectColor;
    }

private:
    void print_vec3(glm::vec3 a) {
        std::cout << "(" << a.x << "," << a.y << "," << a.z << ")";
    }

    Octree* tree;

    virtual glm::vec3 getColor(const int &screen_y, const int &screen_x) = 0;
    virtual glm::vec3 sample(const Ray &ray, const int &recursions, const float &eta1) = 0;



};



#endif
