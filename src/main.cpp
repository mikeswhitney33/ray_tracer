#include <scene.hpp>
#include <shapes/sphere.hpp>
#include <materials/color_material.hpp>

int main() {

    Camera* cam = new Camera(glm::vec3(0.0f, 0.0f, 1.0f));
    Scene* scene = new Scene(glm::vec3(1.0f, 0.5f, 1.0f), cam, 224, 224);

    scene->add_shape(new Sphere(new ColorMaterial(32, 0.1f, 0.5f, glm::vec3(1.0f, 0.0f, 0.0f)), glm::vec3(0.0f, 0.0f, -1.0f), 0.2f));
    scene->save("hello.ppm");
    delete scene;
    // PPM* ppm = new PPM(224, 224);
    // for(int i = 0;i < 224;i++) {
    //     for(int j = 0;j < 224;j++) {
    //         ppm->setPixel(i, j, glm::vec3(1.0f, 0.0f, 0.0f));
    //     }
    // }
    // ppm->toFile("hello.ppm");
    // delete ppm;
    return 0;
}
