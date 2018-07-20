#include <scene.hpp>
#include <shapes/sphere.hpp>
#include <materials/color_material.hpp>
#include <materials/texture_material.hpp>

#include <lights/directional_light.hpp>
#include <lights/point_light.hpp>
#include <lights/area_light.hpp>
#include <scenes/primary_scene.hpp>
#include <scenes/secondary_scene.hpp>
#include <shapes/triangle.hpp>
#include <scenes/path_tracer.hpp>

#include <model.hpp>

int main(int argc, char** argv) {
    if(argc == 2) {
        Camera cam;
        Scene* scene = new PathTracer(glm::vec3(197.0f, 175.0f, 125.0f)/255.0f, cam, 512, 512, 60, 3, 1.003f, 32);

        scene->add_shape(new Sphere(
            new ColorMaterial(glm::vec3(1.0f, 0.0f, 0.0f), 32, 0.4, 0.4),
            glm::vec3(0.0f, 0.0f, 0.0f),
            0.2f
        ));

        scene->add_shape(new Sphere(
            new ColorMaterial(glm::vec3(0.4f, 0.4f, 0.4f), 32, 0.4f, 1.0f, 1.0f),
            glm::vec3(-0.4f, 0.0f, -0.3f),
            0.2f
        ));

        scene->add_shape(new Sphere(
            new ColorMaterial(glm::vec3(0.4f, 0.4f, 0.4f), 32, 0.4f, 1.0f, 0.0f, 0.5f),
            glm::vec3(0.4f, 0.0f, 0.3f),
            0.2f
        ));

        scene->add_shape(new Triangle(
            new ColorMaterial(glm::vec3(1.0f, 1.0f, 1.0f), 32, 0.4, 0.4),
            glm::vec3(-2.0f, -0.2f, -2.0f),
            glm::vec3(-2.0f, -0.2f, 2.0f),
            glm::vec3(2.0f, -0.2f, 2.0f)
        ));
        scene->add_shape(new Triangle(
            new ColorMaterial(glm::vec3(1.0f, 1.0f, 1.0f), 32, 0.4, 0.4),
            glm::vec3(-2.0f, -0.2f, -2.0f),
            glm::vec3(2.0f, -0.2f, -2.0f),
            glm::vec3(2.0f, -0.2f, 2.0f)
        ));


        // glm::vec3(-2.0f, -0.2f, -2.0f),
        // glm::vec3(-2.0f, -0.2f, 2.0f),
        // glm::vec3(2.0f, -0.2f, 2.0f)
        scene->add_shape(new Triangle(
            new ColorMaterial(glm::vec3(0.0f, 0.0f, 1.0f), 32, 0.8f, 0.4f, 0.2f),
            glm::vec3(-2.0f, -0.2f, -2.0f),
            glm::vec3(-2.0f, 2.0f, -2.0f),
            glm::vec3(2.0f, 2.0f, -2.0f)
        ));

        // glm::vec3(-2.0f, -0.2f, -2.0f),
        // glm::vec3(2.0f, -0.2f, -2.0f),
        // glm::vec3(2.0f, -0.2f, 2.0f)
        scene->add_shape(new Triangle(
            new ColorMaterial(glm::vec3(0.0f, 0.0f, 1.0f), 32, 0.8f, 0.4f, 0.2f),
            glm::vec3(-2.0f, -0.2f, -2.0f),
            glm::vec3(2.0f, -0.2f, -2.0f),
            glm::vec3(2.0f, 2.0f, -2.0f)
        ));

        // scene->add_model(Model(
        //         "resources/dragon.obj",
        //         new ColorMaterial(glm::vec3(0.0f, 0.180392f, 0.3647059f), 32, 0.4f, 0.4f),
        //         glm::vec3(0.0f, -0.5f, 0.0f),
        //         glm::vec3(0.0f, 0.0f, 0.0f),
        //         glm::vec3(0.1f, 0.1f, 0.1f)
        // ));

        scene->add_light(new AreaLight(
                glm::vec3(1.0f, 1.0f, 1.0f),
                glm::vec3(-1.0f, 1.0f, -1.0f),
                glm::vec3(1.0f, 1.0f, 1.0f)
        ));
        scene->add_light(new DirectionalLight(
                glm::vec3(1.0f, 1.0f, 1.0f),
                glm::vec3(0.5f, 1.0f, 1.0f)
        ));

        scene->save(argv[1]);

        delete scene;

        return 0;
    }

    else {
        std::cout << "Needs output file location!";
        return -1;
    }
}
