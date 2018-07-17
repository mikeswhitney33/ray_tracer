#include <scene.hpp>
#include <shapes/sphere.hpp>
#include <materials/color_material.hpp>
#include <materials/texture_material.hpp>

#include <lights/directional_light.hpp>
#include <lights/point_light.hpp>
#include <scenes/primary_scene.hpp>
#include <scenes/secondary_scene.hpp>
#include <shapes/triangle.hpp>

#include <model.hpp>

int main(int argc, char** argv) {
    if(argc == 2) {
        Camera cam;
        Scene* scene = new SecondaryScene(glm::vec3(197.0f, 175.0f, 125.0f)/255.0f, cam, 1280, 720, 60, 5, 1.003f);

        scene->add_model(Model(
                "resources/dragon.obj",
                new ColorMaterial(glm::vec3(0.0f, 0.180392f, 0.3647059f), 32, 0.4f, 0.4f),
                glm::vec3(0.0f, -0.5f, 0.0f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.1f, 0.1f, 0.1f)
        ));

        scene->add_light(new PointLight(
                glm::vec3(1.0f, 1.0f, 1.0f),
                glm::vec3(0.0f, 1.0f, 0.0f)
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
