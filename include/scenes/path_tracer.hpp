#ifndef PATH_TRACER_H
#define PATH_TRACER_H

#include <scene.hpp>
#include <utils.hpp>

class PathTracer : public Scene {
public:
    PathTracer(
        const glm::vec3 &bkgd,
        const Camera &cam,
        const int &w,
        const int &h,
        const float &fov,
        const int &recursions,
        const float &eta,
        const int &paths
    )
    : Scene(bkgd, cam, w, h, fov, recursions, eta) {
        num_paths = paths;
    }
private:
    int num_paths;

    virtual glm::vec3 sample(const Ray &ray, const int &recursions, const float &eta1) {
        glm::vec3 normal; float t; int i; glm::vec2 uv; Geometry* shape;
        if(intersect(ray, t, normal, uv, shape) && recursions < max_recursions) {
            if(shape->isLight()) {
                return shape->material->getObjectColor(uv);
            }
            float factor = (max_recursions - recursions) / max_recursions;
            glm::vec3 pt = ray.eval(t);
            std::vector<bool> shadows = inShadow(pt);

            glm::vec3 direct_lighting;
            for(int i = 0;i < shadows.size();i++) {
                if(!shadows[i]) {
                    //directLighting = vis * lightIntensity * std::max(0.f, hitNormal.dotProduct(-lightDir));
                    direct_lighting += lights[i]->getColor();// * std::max(0.0f, dot(glm::normalize(normal), -glm::normalize(lights[i]->getDirection(pt))));
                }
            }
            direct_lighting /= shadows.size();

            Material* mat = shape->material;
            float diffusek = 1 - mat->specularStrength;
            float reflectk = mat->reflectiveStrength;
            float refractk = mat->refractiveStrength;
            glm::vec3 secondary_color;

            int N = num_paths / (recursions + 1);
            for(int i = 0;i < N;i++) {
                float chance = random_float(0, diffusek + reflectk + refractk);
                if(chance <= diffusek) {
                    //Send diffuse ray
                    secondary_color += diffuse(pt, ray.dir, normal, recursions, eta1);
                }
                else if(chance <= diffusek + reflectk) {
                    // Send reflect Ray
                    secondary_color += reflect(pt, ray.dir, normal, recursions, eta1);
                }
                else {
                    // Send Refract Ray
                    secondary_color += refract(pt, ray.dir, normal, eta1, mat->indexOfRefraction, recursions);
                }
            }
            secondary_color /= (float)N;

            return (direct_lighting / (float)M_PI + 2.0f * secondary_color) * mat->getObjectColor(uv);//mat->getColor(pt, normal,  ray.orig, uv,  lights, shadows);
        }
        else {
            if(ray.type == DIFFUSE) {
                return glm::vec3();
            }
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
        glm::vec3 rd = glm::normalize(glm::vec3(pt.x, pt.y, pt.z) - camera.Position);
        glm::vec3 r0 = camera.Position;
        glm::vec3 color(0.0f, 0.0f, 0.0f);
        return sample(Ray(r0, rd), 0, initial_eta);
        // for(int i = 0;i < num_paths;i++) {
        //     color += sample(Ray(r0, rd), 0, initial_eta) / (float) num_paths;
        // }
        // return color;
    }

};


#endif
