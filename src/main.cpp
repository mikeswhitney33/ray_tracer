#include <scene.hpp>
#include <shapes/sphere.hpp>
#include <materials/color_material.hpp>
#include <materials/texture_material.hpp>

#include <lights/directional_light.hpp>
#include <lights/point_light.hpp>
#include <scenes/primary_scene.hpp>
#include <scenes/secondary_scene.hpp>
#include <shapes/triangle.hpp>

#include <fstream>

int main(int argc, char** argv) {
    if(argc == 2) {
        Camera* cam = new Camera();
        Scene* scene = new SecondaryScene(glm::vec3(0.0f, 0.5f, 1.0f), cam, 512, 512, 60, 60, 5, 1.003);

        scene->add_shape(new Sphere(new TextureMaterial("resources/awesomeface.png", 32, 0.1f, 0.25f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f), 0.2f));

        scene->add_shape(new Sphere(new ColorMaterial(glm::vec3(0.1f, 0.3f, 0.7f), 32, 0.1f, 0.25f, 0.0f, 0.5f, 1.5f), glm::vec3(0.1f, -0.1f, 0.3f), 0.1f));

        scene->add_shape(new Triangle(new ColorMaterial(glm::vec3(0.0f, 1.0f, 0.0f), 32, 0.1f, 0.75f, 0.5f), glm::vec3(-0.5f, -0.3f, -0.5f), glm::vec3(0.5f, -0.3f, -0.5f), glm::vec3(0.5f, -0.3f, 0.5f)));
        scene->add_shape(new Triangle(new ColorMaterial(glm::vec3(0.0f, 1.0f, 0.0f), 32, 0.1f, 0.75f, 0.5f), glm::vec3(-0.5f, -0.3f, -0.5f), glm::vec3(-0.5f, -0.3f, 0.5f), glm::vec3(0.5f, -0.3f, 0.5f)));

        scene->add_light(new PointLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

        // std::ofstream f(argv[1]);

        std::cout << "Saving.." << std::endl;
        PPM* ppm = scene->save();
        std::cout << "Saved.." << std::endl;

        delete scene;
        std::cout << "ToFile.." << std::endl;
        ppm->toFile(argv[1]);

        // f.close();

        // std::cout << "Saved" << std::endl;
        // delete scene;

        return 0;
    }

    else {
        std::cout << "Needs output file location!";
        return -1;
    }
}
