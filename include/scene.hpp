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

class Scene {
public:
    Scene(glm::vec3 bkgd, Camera* cam, int w, int h) {
        backgroundColor = bkgd;
        camera = cam;
        width = w;
        height = h;
        viewport = glm::vec4(0.0f, 0.0f, w, h);
        projection = glm::perspective(glm::radians(60.0f), (float)width / (float)height, 0.1f, 100.0f);
    }

    ~Scene() {
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
    }

    void add_light(Light* light) {
        lights.push_back(light);
    }

    void save(const char* filename) {
        PPM* ppm = new PPM(width, height);
        for(int y = 0;y < height;y++) {
            // std::cout << y << "/" << height << std::endl;
            for(int x = 0;x < width;x++) {
                ppm->setPixel(y, x, getColor(y, x));
            }
        }
        ppm->toFile(filename);
        delete ppm;
    }

private:
    std::vector<Geometry*> shapes;
    std::vector<Light*> lights;
    glm::vec3 backgroundColor;
    Camera* camera;
    int width, height;
    glm::vec4 viewport;
    glm::mat4 projection;


    glm::vec3 sample(glm::vec3 r0, glm::vec3 rd) {
        float t_min = 9999.0f;
        glm::vec3 normal;
        float t;
        int shape_index = -1;
        for(int i = 0;i < shapes.size();i++) {
            if(shapes[i]->intersect(r0, rd, normal, t)) {
                if(t < t_min) {
                    t_min - t;
                    shape_index = i;
                }
            }
        }
        if(shape_index > -1) {
            Material* mat = shapes[shape_index]->getMaterial();
            return mat->getColor(r0 + rd*t, normal, r0, lights);
        }
        else {
            return backgroundColor;
        }
    }

    void print_mat4(glm::mat4 a) {
        for(int i = 0;i < 4;i++) {
            for(int j = 0;j < 4;j++) {
                std::cout << a[i][j] << "\t";
            }
            std::cout << std::endl;
        }
    }
    glm::vec3 getColor(int screen_y, int screen_x) {


        float y = 2.0f * screen_x / width - 1;
        float x = 2.0f * screen_y / height - 1;

        glm::vec4 screenPos = glm::vec4(x, y, 1.0f, 1.0f);

        glm::mat4 projectview = projection * camera->GetViewMatrix();
        glm::mat4 viewProjectionInverse = glm::inverse(projectview);

        glm::vec4 worldPos = viewProjectionInverse * screenPos;
        // worldPos = (worldPos / worldPos.z);
        glm::vec3 wPos = worldPos / worldPos.w;
        wPos /= wPos.z;
        wPos -= camera->Position;
        // std::cout << "(" << screen_y << "," << screen_x << ") --> (" << camera->Position.x << "," << camera->Position.y << "," << camera->Position.z << ")->(" << wPos.x << ", " << wPos.y << "," << wPos.z << ")" << std::endl;
        return sample(camera->Position, glm::vec3(worldPos));
        // glm::mat4 view = camera->GetViewMatrix();
        // glm::vec3 screenPos(x, y, 0);
        // glm::vec3 worldPos = glm::unProject(screenPos, view, projection, viewport);
        // worldPos = worldPos / (worldPos.z);
        // return sample(camera->Position, worldPos);
    }
};


#endif
