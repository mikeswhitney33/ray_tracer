#ifndef SECONDARY_SCENE_H
#define SECONDARY_SCENE_H

#include <scene.hpp>

class SecondaryScene : public Scene {
public:
    SecondaryScene(glm::vec3 bkgd, Camera* cam, int w, int h, float fov_x, float fov_y, int recursions, float eta)
        : Scene(bkgd, cam, w, h, fov_x, fov_y, recursions, eta) {

    }

    virtual ~SecondaryScene() {

    }
private:
    virtual glm::vec3 sample(glm::vec3 r0, glm::vec3 rd, int recursions, float eta1) {
        glm::vec3 normal;
        float t;
        int i;
        if(recursions < max_recursions && intersect(r0, rd, t, normal, i)) {
            Material* mat = shapes[i]->getMaterial();
            std::vector<bool> shadows = inShadow(r0 + t * rd);

            glm::vec3 matColor = mat->getColor(r0 + rd * t, normal, r0, lights, inShadow(r0 + t * rd));

            if(mat->reflectiveStrength > 0) {
                matColor += mat->reflectiveStrength * reflect(r0 + rd * t, rd, glm::normalize(normal), recursions, eta1);
            }

            if(mat->refractiveStrength > 0) {
                matColor += mat->refractiveStrength * refract(r0 + rd * t, rd, glm::normalize(normal), eta1, mat->indexOfRefraction, recursions);
            }

            return matColor;
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
