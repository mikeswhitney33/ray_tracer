#ifndef SECONDARY_SCENE_H
#define SECONDARY_SCENE_H

#include <scene.hpp>
#include <ray.hpp>

class SecondaryScene : public Scene {
public:
    SecondaryScene(const glm::vec3 &bkgd, const Camera &cam, const int &w, const int &h, const float &fov, const int &recursions, const float &eta)
        : Scene(bkgd, cam, w, h, fov, recursions, eta) {}

    virtual ~SecondaryScene() {}
private:
    virtual glm::vec3 sample(const Ray &ray, const int &recursions, const float &eta1) {
        glm::vec3 normal;
        float t;
        int i;
        glm::vec2 uv;
        Geometry* shape;
        if(recursions < max_recursions && intersect(ray, t, normal, uv, shape)) {
            Material* mat = shape->material;
            glm::vec3 inter = ray.eval(t);
            std::vector<bool> shadows = inShadow(inter);

            glm::vec3 matColor = mat->getColor(inter, normal, ray.dir, uv, lights, inShadow(inter));

            if(mat->reflectiveStrength > 0) {
                matColor += mat->reflectiveStrength * reflect(inter, ray.dir, glm::normalize(normal), recursions, eta1);
            }

            if(mat->refractiveStrength > 0) {
                matColor += mat->refractiveStrength * refract(inter, ray.dir, glm::normalize(normal), eta1, mat->indexOfRefraction, recursions);
            }

            return matColor;
        }
        else {
            return backgroundColor;
        }
    }

    virtual glm::vec3 getColor(const int &screen_y, const int &screen_x) {
        float u, v, w;
        u = screen_x * 2 * u_max / width - u_max;
        v = screen_y * 2 * v_max / height - v_max;
        w = 0;

        glm::vec4 pt = camera.vrc * glm::vec4(u, v, w, 1.0f);
        glm::vec3 rd = glm::vec3(pt.x, pt.y, pt.z) - camera.Position;
        glm::vec3 r0 = camera.Position;
        return sample(Ray(r0, glm::normalize(rd)), 0, initial_eta);
    }
};

#endif
