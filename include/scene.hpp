#ifndef SCENE_H
#define SCENE_H

#include <glm/glm.hpp>
#include <geometry.hpp>
#include <camera.hpp>
#include <light.hpp>
#include <vector>
#include <ppm.hpp>
#include <material.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>

#define SMALL_NUM   ((float)pow(2, -13))


// #define clip(a, min_val, max_val) (a > max_val ? max_val : a < min_val ? min_val : a)

class Scene {
public:
    Scene(glm::vec3 bkgd, Camera* cam, int w, int h, float fov_x, float fov_y, int recursions, float eta) {
        backgroundColor = bkgd;
        camera = cam;
        width = w;
        height = h;
        u_max = 2 * (float) tan(glm::radians(fov_x / 2.0f));
        v_max = 2 * (float) tan(glm::radians(fov_y / 2.0f));
        max_recursions = recursions;
        initial_eta = eta;
    }

    virtual ~Scene() {
        std::cout << "Destructing Scene!" << std::endl;

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

    PPM* save() {
        std::cout << "Shapes: " << shapes.size() << std::endl;
        PPM* ppm = new PPM(width, height);
        glm::vec3 grid[height][width];
        float max_val = 0;
        float min_val = 9999.0f;
        std::cout << "Tracing..." << std::endl;
        for(int y = 0;y < height;y++) {
            for(int x = 0;x < width;x++) {
                glm::vec3 color = getColor(y, x) * 255.0f;
                for(int i = 0;i < 3;i++) {
                    if(color[i] > max_val) {
                        max_val = color[i];
                    }
                    if(color[i] < min_val) {
                        min_val = color[i];
                    }
                }
                grid[y][x] = color;
            }
        }
        std::cout << "Regularizing..." << std::endl;

        if(max_val > 255) {
            float factor = log(255) / log(max_val);
            for(int y = 0;y < height;y++) {
                for(int x = 0;x < width;x++){
                    glm::vec3 color = glm::pow(grid[y][x], glm::vec3(factor, factor, factor));
                    // (grid[y][x] - min_val)/(max_val-min_val)
                    ppm->setPixel(height - y, width - x, (unsigned char) color.x, (unsigned char) color.y, (uint8) color.z);
                }
            }
        }
        return ppm;
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
