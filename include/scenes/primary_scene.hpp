#ifndef PRIMARY_SCENE_H
#define PRIMARY_SCENE_H

#include <scene.hpp>

/*
 * Class: Primary Scene
 * --------------------
 * A class used for a Basic Ray Traced scene. It only sends primary rays
 *    so no shadows, reflections, or refrasctions will be seen.
 */
class PrimaryScene : public Scene {
public:
    /*
     * Constructor
     * -----------
     * Constructs a new Primary Scene
     *
     * Parameters:
     *      vec3 bkgd: The background color
     *      Camera cam: a pointer to an instance of the Camera class
     *      int w: the width of the image
     *      int h: the height of the image
     *      float fov_x: the Field of view in the x direction
     *      float fov_y: the Field fo view in the y direction
     */
    PrimaryScene(const glm::vec3 &bkgd, const Camera &cam, const int &w, const int &h, const float &fov):Scene(bkgd, cam, w, h, fov, 1, 1.003) {}
    virtual ~PrimaryScene() {}
private:
    virtual glm::vec3 sample(const Ray &ray, const int &recursions, const float &eta1) {
        glm::vec3 normal;
        float t;
        int idx;
        glm::vec2 uv;
        Geometry* shape;
        if(intersect(ray, t, normal, uv, shape)) {
            Material* mat = shape->material;
            std::vector<bool> shadows;
            for(int i = 0;i < lights.size();i++) {
                shadows.push_back(false);
            }
            return mat->getColor(ray.eval(t), normal, ray.dir, uv, lights, shadows);
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
        return sample(Ray(r0, rd), 0, initial_eta);
    }
};

#endif
