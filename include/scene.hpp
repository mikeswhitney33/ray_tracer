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
#include <utils.hpp>


#define SMALL_NUM   ((float)pow(2, -13))


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
        for(int i = 0;i < lights.size();i++) {
            delete lights[i];
        }
        delete tree;
    }

    void add_shape(Geometry* shape) {
        tree->add_shape(shape);
    }


    void add_model(const Model &model) {
        std::vector<Geometry*> s = model.getShapes();
        for(int i = 0;i < s.size();i++) {
            add_shape(s[i]);
        }
    }

    void add_light( Light* light) {
        lights.push_back(light);
    }

    static void set_color(Scene* context, int y, int x, cv::Mat m) {
        glm::vec3 color = 255.0f * context->getColor(y, x);
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

    virtual glm::vec3 diffuse(const glm::vec3 &pt, glm::vec3 rd, const glm::vec3 &normal, const int  &recursions, const float &eta1) {
        rd = glm::vec3(random_float(-1.0f, 1.0f), random_float(-1.0f, 1.0f), random_float(-1.0f, 1.0f));
        if(glm::dot(rd, normal) > 0) {
            rd = -rd;
        }
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

private:
    void print_vec3(glm::vec3 a) {
        std::cout << "(" << a.x << "," << a.y << "," << a.z << ")";
    }

    Octree* tree;

    virtual glm::vec3 getColor(const int &screen_y, const int &screen_x) = 0;
    virtual glm::vec3 sample(const Ray &ray, const int &recursions, const float &eta1) = 0;
};

#endif
