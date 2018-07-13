#ifndef SCENE_H
#define SCENE_H

#include <glm/glm.hpp>
#include <geometry.hpp>
#include <camera.hpp>
#include <light.hpp>
#include <vector>
#include <material.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <ctime>
#include <chrono>

#include <opencv2/core.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/base.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/hal/interface.h>
#include <opencv2/imgproc.hpp>


#define SMALL_NUM   ((float)pow(2, -13))


// #define clip(a, min_val, max_val) (a > max_val ? max_val : a < min_val ? min_val : a)

class Scene {
public:
    Scene(glm::vec3 bkgd, Camera* cam, int w, int h, float fov, int recursions, float eta) {
        backgroundColor = bkgd;
        camera = cam;
        width = w;
        height = h;
        float fov_x, fov_y;
        fov_x = fov;
        fov_y = fov * ((w > h) ? (float)h / (float)w : (float) w / (float)h);

        u_max = 2 * (float) tan(glm::radians(fov_y / 2.0f));
        v_max = 2 * (float) tan(glm::radians(fov_x / 2.0f));
        max_recursions = recursions;
        initial_eta = eta;
    }

    virtual ~Scene() {

        delete camera;
        for(int i = 0;i < shapes.size();i++) {
            delete shapes[i];
        }
        for(int i = 0;i < lights.size();i++) {
            delete lights[i];
        }
    }

    void add_shape(Geometry* shape) {
        shapes.push_back(shape);
        // std::cout << "Shape Added! Size: " << shapes.size() << std::endl;
    }

    void add_light(Light* light) {
        lights.push_back(light);
    }

    void save(const char* filename) {

        // cv::Mat m(width, height , CV_32FC(3), cv::Scalar(0.0f, 0.0f, 0.0f));
        cv::Mat m;
        m.create(width, height, CV_32FC(3));

        for(int y = 0;y < height;y++) {
            std::cout << y << "/" << height << " -- " << (100 * y / height)  << "%       \r" << std::flush;
            for(int x = 0;x < width;x++) {
                glm::vec3 color = 255.0f * getColor(y, x);
                m.at<cv::Vec3f>(y, x).val[2] = color.x;
                m.at<cv::Vec3f>(y, x).val[1] = color.y;
                m.at<cv::Vec3f>(y, x).val[0] = color.z;
            }
        }
        cv::normalize(m, m, 0, 255, cv::NORM_MINMAX, CV_32FC3);


        std::vector<int> params;
        params.push_back(cv::IMWRITE_PNG_COMPRESSION);
        params.push_back(9);

        cv::imwrite(filename, m);
        std::cout << std::endl;
    }

protected:
    std::vector<Geometry*> shapes;
    std::vector<Light*> lights;
    glm::vec3 backgroundColor;
    Camera* camera;
    int width, height;
    float u_max, v_max;
    int max_recursions;
    float initial_eta;

    virtual glm::vec3 reflect(glm::vec3 pt, glm::vec3 rd, glm::vec3 normal, int recursions, float eta1) {
        rd = glm::reflect(rd, normal);
        return sample(pt + rd * SMALL_NUM, rd, recursions + 1, eta1);
    }

    virtual glm::vec3 refract(glm::vec3 pt, glm::vec3 rd, glm::vec3 normal, float eta1, float eta2, int recursions) {
        float ndoti = glm::dot(rd, normal);
        if(ndoti < 0) {
            ndoti = -ndoti;
        } else {
            normal = -normal;
            std::swap(eta1, eta2);
        }
        float eta = eta1 / eta2;
        rd = glm::refract(rd, normal, eta);
        return sample(pt + rd * SMALL_NUM, rd, recursions + 1, eta2);
    }

    virtual std::vector<bool> inShadow(glm::vec3 r0) {
        std::vector<bool> shadows;
        int shape_index;
        for(int i = 0;i < lights.size();i++) {
            float light_dist = lights[i]->getDistance(r0);
            glm::vec3 rd = glm::normalize(lights[i]->getDirection(r0));
            float t;
            glm::vec3 n;
            if(intersect(r0 + SMALL_NUM * rd, rd, t, n, shape_index)) {
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

    virtual bool intersect(glm::vec3 r0, glm::vec3 rd, float &t, glm::vec3 &normal, int &shape_index) {
        t = 9999.0f;
        shape_index = -1;
        for(int i = 0;i < shapes.size();i++) {
            glm::vec3 tmp_normal;
            float t_tmp;
            if(shapes[i]->intersect(r0, rd, tmp_normal, t_tmp)) {
                if(t_tmp < t) {
                    t = t_tmp;
                    shape_index = i;
                    normal = tmp_normal;
                }
            }
        }
        return shape_index > -1;
    }

private:
    void print_vec3(glm::vec3 a) {
        std::cout << "(" << a.x << "," << a.y << "," << a.z << ")";
    }

    virtual glm::vec3 getColor(int screen_y, int screen_x) = 0;
    virtual glm::vec3 sample(glm::vec3 r0, glm::vec3 rd, int recursions, float eta1) = 0;
};

#endif
