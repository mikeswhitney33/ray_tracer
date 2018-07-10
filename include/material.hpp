#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>
#include <vector>
#include <light.hpp>
#include <math.h>


// MATERIAL num KdR KdG KdB KsR KsG KsB N Kd Kt
class Material {
public:
    Material(int n, float ka, float ks) {
        phong = n;
        ambientStrength = ka;
        specularStrength = ks;
    }

    glm::vec3 getColor(glm::vec3 interPos, glm::vec3 normal, glm::vec3 viewPos, std::vector<Light*> lights) {
        // glm::vec3 objectColor = getObjectColor();
        // glm::vec3 final_color = glm::vec3(0.0f, 0.0f, 0.0f);
        // for(int i = 0;i < lights.size();i++) {
        //     Light* light = lights[i];
        //     glm::vec3 lightColor = light->getColor();
        //
        //
        //     glm::vec3 ambient = ambientStrength * lightColor;
        //
        //     glm::vec3 norm = glm::normalize(normal);
        //     glm::vec3 lightDir = glm::normalize(light->getDirection(interPos));
        //     float diff = std::max(glm::dot(norm, lightDir), 0.0f);
        //     glm::vec3 diffuse = diff * lightColor;
        //
        //     glm::vec3 viewDir = glm::normalize(viewPos - interPos);
        //     glm::vec3 reflectDir = glm::reflect(-lightDir, norm);
        //     float spec = pow(std::max(glm::dot(viewDir, reflectDir), 0.0f), phong);
        //     glm::vec3 specular = specularStrength * spec * lightColor;
        //
        //     final_color += (ambient + diffuse + specular) * objectColor;
        // }
        // return final_color;
        return getObjectColor();
    }
private:
    virtual glm::vec3 getObjectColor() = 0;
    int phong;
    float ambientStrength;
    float specularStrength;
};

#endif
