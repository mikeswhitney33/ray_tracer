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
        Scene* scene = new SecondaryScene(glm::vec3(0.0f, 0.5f, 1.0f), cam, 1920, 1080, 60, 33.75f, 5, 1.003);

        scene->add_shape(
                new Sphere(
                        new TextureMaterial(
                                "resources/bowling_ball.png",
                                32,//PHong
                                0.4f, // Ambient
                                0.75f, // Specular
                                0.0f),
                        glm::vec3(0.0f, -0.1f, 0.0f),
                        0.2f
                ));

        scene->add_shape(
                new Sphere(
                        new ColorMaterial(
                                glm::vec3(0.7f, 0.3f, 0.3f),
                                4, //Phong Constant
                                0.2f, //Ambient Strength
                                0.75f, //Specular Strength
                                0.5f, //Reflective Strength
                                0.5f, //Refractive Strength
                                1.25f), //Index of Refraction
                        glm::vec3(0.1f, -0.2f, 0.3f),
                        0.1f
                ));

        scene->add_shape(
                new Triangle(
                        new TextureMaterial("resources/chess.png", 32, 0.1f, 0.75f, 0.0f),
                        glm::vec3(-1.0f, -0.3f, -1.0f), glm::vec3(1.0f, -0.3f, -1.0f), glm::vec3(1.0f, -0.3f, 1.0f),
                        glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f)
                ));
        scene->add_shape(
                new Triangle(
                        new TextureMaterial("resources/chess.png", 32, 0.1f, 0.75f, 0.0f),
                        glm::vec3(-1.0f, -0.3f, -1.0f), glm::vec3(-1.0f, -0.3f, 1.0f), glm::vec3(1.0f, -0.3f, 1.0f),
                        glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec2(1.0f, 1.0f)
                ));

        scene->add_light(new PointLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.3f, 0.5f, 0.1f)));
        scene->add_light(new PointLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(-0.3f, 0.5f, 0.1f)));

        scene->save(argv[1]);

        delete scene;

        return 0;
    }

    else {
        std::cout << "Needs output file location!";
        return -1;
    }
}
