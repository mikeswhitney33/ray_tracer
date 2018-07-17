#ifndef SCENE_H
#define SCENE_H

#include <glm/glm.hpp>
#include <geometry.hpp>
#include <camera.hpp>
#include <light.hpp>
#include <vector>
#include <material.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <model.hpp>
#include <fstream>
#include <timing.hpp>
#include <thread>
#include <bounding_box.hpp>
#include <ray.hpp>
#include <octree.hpp>

#include <opencv2/core.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/base.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/hal/interface.h>
#include <opencv2/imgproc.hpp>


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


        // left_box = BoundingBox(glm::vec3(-999.0f, -999.0f, -999.0f), glm::vec3(0, 999.0f, 999.0f));
        // right_box = BoundingBox(glm::vec3(0.0f, -999.0f, -999.0f), glm::vec3(999.0f, 999.0f, 999.0f));

    }

    virtual ~Scene() {
        for(int i = 0;i < left_shapes.size();i++) {
            delete left_shapes[i];
        }
        for(int i = 0;i < right_shapes.size();i++) {
            delete right_shapes[i];
        }
        for(int i = 0;i < middle_shapes.size();i++) {
            delete middle_shapes[i];
        }
        for(int i = 0;i < lights.size();i++) {
            delete lights[i];
        }
        delete tree;
    }

    void add_shape(Geometry* shape) {
        tree->add_shape(shape);
        // bool left, right;
        // left = left_box.intersect(shape->bounding_box);
        // right = right_box.intersect(shape->bounding_box);
        // if(left && right) {
        //     middle_shapes.push_back(shape);
        // }
        // else if(left) {
        //     left_shapes.push_back(shape);
        // }
        // else if(right) {
        //     right_shapes.push_back(shape);
        // }
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
        std::cout << "Saving " << filename << std::endl;
        // float min_vals[3];
        // memset(min_vals, 9999.0f, 3 * sizeof(float));
        // float max_vals[3];
        // memset(max_vals, -9999.0f, 3 * sizeof(float));
        // std::cout << "Memsets" << std::endl;
        // for(int i = 0;i < shapes.size();i++) {
        //     for(int j = 0; j < 3;j++) {
        //         if(shapes[i]->bounding_box->lowExtent[j] < min_vals[j]) {
        //             min_vals[j] = shapes[i]->bounding_box->lowExtent[j];
        //         }
        //         if(shapes[i]->bounding_box->highExtent[j] > max_vals[j]) {
        //             max_vals[j] = shapes[i]->bounding_box->highExtent[j];
        //         }
        //     }
        // }



        // for(int i = 0;i < shapes.size();i++) {
        //     bool left = shapes[i]->bounding_box->intersect(left_box);
        //     bool right = shapes[i]->bounding_box->intersect(right_box);
        //     if(left && right) {
        //         middle_shapes.push_back(shapes[i]);
        //     }
        //     else if(left) {
        //         left_shapes.push_back(shapes[i]);
        //     }
        //     else {
        //         right_shapes.push_back(shapes[i]);
        //     }
        // }


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
    }

protected:
    // std::vector<Geometry*> shapes;

    BoundingBox left_box;
    BoundingBox right_box;


    std::vector<Geometry*> left_shapes;
    std::vector<Geometry*> right_shapes;
    std::vector<Geometry*> middle_shapes;

    std::vector<Light*> lights;
    glm::vec3 backgroundColor;
    Camera camera;
    int width, height;
    float u_max, v_max;
    int max_recursions;
    float initial_eta;

    virtual glm::vec3 reflect(glm::vec3 pt, glm::vec3 rd, glm::vec3 normal, int recursions, float eta1) {
        rd = glm::reflect(rd, normal);
        return sample(Ray(pt + rd * SMALL_NUM, rd), recursions + 1, eta1);
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
        return sample(Ray(pt + rd * SMALL_NUM, rd), recursions + 1, eta2);
        // return sample(pt + rd * SMALL_NUM, rd, recursions + 1, eta2);
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
            // if(intersect(r0 + SMALL_NUM * rd, rd, t, n, uv, shape)) {
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
        // t = 999.0f;
        // bool shape_chosen = false;
        // glm::vec3 n;
        // float t_tmp;
        // glm::vec2 uv_tmp;
        // bool left, right;
        // left = left_box.intersect(ray);
        // right = right_box.intersect(ray);
        // if(left && right) {
        //     for(int i = 0;i < middle_shapes.size();i++) {
        //         if(middle_shapes[i]->intersect(ray, n, t_tmp, uv_tmp)) {
        //             if(t_tmp < t) {
        //                 shape_chosen = true;
        //                 shape = middle_shapes[i];
        //                 normal = n;
        //                 uv = uv_tmp;
        //                 t = t_tmp;
        //             }
        //         }
        //     }
        // }
        // if(left) {
        //     for(int i = 0;i < left_shapes.size();i++) {
        //         if(left_shapes[i]->intersect(ray, n, t_tmp, uv_tmp)) {
        //             if(t_tmp < t) {
        //                 shape_chosen = true;
        //                 shape = left_shapes[i];
        //                 normal = n;
        //                 uv = uv_tmp;
        //                 t = t_tmp;
        //             }
        //         }
        //     }
        // }
        // if(right) {
        //     for(int i = 0;i < right_shapes.size();i++) {
        //         if(right_shapes[i]->intersect(ray, n, t_tmp, uv_tmp)) {
        //             if(t_tmp < t) {
        //                 shape_chosen = true;
        //                 shape = right_shapes[i];
        //                 normal = n;
        //                 uv = uv_tmp;
        //                 t = t_tmp;
        //             }
        //         }
        //     }
        // }
        // return shape_chosen;
        // t = 9999.0f;
        // shape_index = -1;
        // for(int i = 0;i < shapes.size();i++) {
        //     glm::vec3 tmp_normal;
        //     float t_tmp;
        //     glm::vec2 tmp_uv;
        //     if(shapes[i]->intersect(r0, rd, tmp_normal, t_tmp, tmp_uv)) {
        //         if(t_tmp < t) {
        //             t = t_tmp;
        //             shape_index = i;
        //             normal = tmp_normal;
        //             uv = tmp_uv;
        //         }
        //     }
        // }
        // return shape_index > -1;
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
