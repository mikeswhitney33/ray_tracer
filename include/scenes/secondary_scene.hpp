#ifndef SECONDARY_SCENE_H
#define SECONDARY_SCENE_H

#include <scene.hpp>
#include <ray.hpp>

/*
 * Class: Secondary Scene
 * --------------------
 * A class used for a Basic ray traced scene that uses both primary rays and
 *       secondary rays.  Secondary rays include shadow rays, reflection rays
 *       and transmission rays.
 */
class SecondaryScene : public Scene {
public:
    /*
     * Constructor
     * -----------
     * Constructs a new Secondary Scene
     *
     * Parameters:
     *      vec3 bkgd: The background color
     *      Camera cam: a pointer to an instance of the Camera class
     *      int w: the width of the image
     *      int h: the height of the image
     *      float fov: the Field of view in the x direction (this will be used
     *            to calculate the y direction)
     *      int recursions: the total number of recursions to sample for.
     *      float eta: the default index of refraction.
     */
    SecondaryScene(
        const glm::vec3 &bkgd,
        const Camera &cam,
        const int &w,
        const int &h,
        const float &fov,
        const int &recursions,
        const float &eta)
        : Scene(bkgd, cam, w, h, fov, recursions, eta) {}

    /*
     * Destructor
     * ----------
     * removes allocated resources, for this case there are none.
     */
    virtual ~SecondaryScene() {}
private:
    /*
     * Method: sample
     * ----------------
     * samples a color given a ray after sending out shadow, reflective,
     *       and refractive rays.
     *
     * Parameters:
     *      Ray ray: the ray we are sampling with.
     *      int recursions: the number of recursions we have done so far.
     *      float eta1: the current material's index of refraction.
     * Returns: the color from the given ray.
     */
    virtual glm::vec3 sample(
        const Ray &ray,
        const int &recursions,
        const float &eta1) {

        glm::vec3 normal;
        float t;
        int i;
        glm::vec2 uv;
        Geometry* shape;
        if(recursions < max_recursions &&
            intersect(ray, t, normal, uv, shape)) {
            Material* mat = shape->material;
            glm::vec3 inter = ray.eval(t);
            std::vector<bool> shadows = inShadow(inter);

            glm::vec3 matColor = mat->getColor(
                inter, normal, ray.dir, uv, lights, inShadow(inter));
            normal = glm::normalize(normal);

            if(mat->reflectiveStrength > 0) {
                matColor += mat->reflectiveStrength * reflect(
                    inter, ray.dir, normal, recursions, eta1);
            }

            if(mat->refractiveStrength > 0) {
                float eta = mat->indexOfRefraction;
                glm::vec3 re_color = refract(
                    inter, ray.dir, normal, eta1, eta, recursions);
                matColor += mat->refractiveStrength * re_color;
            }

            return matColor;
        }
        else {
            return backgroundColor;
        }
    }

    /*
     * Method: getColor
     * ------------------
     * retrieves a color from a given screen coordinates.
     *
     * Parameters:
     *      int screen_y: the y coordinate on the screen.
     *      int screen_x: the x coordinate on the screen.
     * Returns: the color of the given screen coordinates.
     */
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
