#include <scene.hpp>
#include <shapes/sphere.hpp>
#include <materials/color_material.hpp>
#include <lights/directional_light.hpp>
#include <scenes/primary_scene.hpp>

int main(int argc, char** argv) {

    if(argc == 2) {
        Scene* scene = new PrimaryScene(glm::vec3(0.0f, 0.5f, 1.0f), Camera(), 512, 512, 60.0f);

        scene->add_shape(new Sphere(new ColorMaterial(glm::vec3(1.0f, 0.0f, 0.0f), 32, 0.1f, 0.75f), glm::vec3(0.0f, 0.0f, -1.0f), 0.2f));
        scene->add_shape(new DirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
        scene->add_shape(new DirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f)));


        scene->save(argv[1]);
        delete scene;

        return 0;
    }

    else {
        std::cout << "Needs output file location!";
        return -1;
    }
}
