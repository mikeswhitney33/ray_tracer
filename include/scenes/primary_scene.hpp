#ifndef PRIMARY_SCENE_H
#define PRIMARY_SCENE_H

#include <scene.hpp>

class PrimaryScene : public Scene {
public:
    // Scene(glm::vec3 bkgd, Camera* cam, int w, int h, float fov_x, float fov_y)
    PrimaryScene(glm::vec3 bkgd, Camera* cam, int w, int h, float fov_x, float fov_y):Scene(bkgd, cam, w, h, fov_x, fov_y, 1, 1.003) {

    }

    virtual ~PrimaryScene() {
        std::cout << "Destructing Primary scene!" << std::endl;

    }
private:
    virtual glm::vec3 sample(glm::vec3 r0, glm::vec3 rd, int recursions, float eta1) {
        glm::vec3 normal;
        float t;
        int idx;
        if(intersect(r0, rd, t, normal, idx)) {
            Material* mat = shapes[idx]->getMaterial();
            std::vector<bool> shadows;
            for(int i = 0;i < lights.size();i++) {
                shadows.push_back(false);
            }
            return mat->getColor(r0 + rd * t, normal, r0, lights, shadows);
        }
        else {
            return backgroundColor;
        }
    }

    virtual glm::vec3 getColor(int screen_y, int screen_x) {
        float u, v, w;
        u = screen_x * 2 * u_max / width - u_max;
        v = screen_y * 2 * v_max / height - v_max;
        w = 0;

        glm::vec4 pt = camera->vrc * glm::vec4(u, v, w, 1.0f);
        glm::vec3 rd = glm::vec3(pt.x, pt.y, pt.z) - camera->Position;
        glm::vec3 r0 = camera->Position;
        return sample(r0, glm::normalize(rd), 0, initial_eta);
    }
};

#endif
