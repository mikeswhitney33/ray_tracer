#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>
#include <vector>
#include <light.hpp>
#include <math.h>
#include <iostream>

class Material {
public:
    Material(int n, float ka, float ks, float k_reflect=0.0f, float k_refract=0.0f, float i_refract=1.003) {
        phong = n;
        ambientStrength = ka;
        specularStrength = ks;
        reflectiveStrength = k_reflect;
        refractiveStrength = k_refract;
        indexOfRefraction = i_refract;
    }

    Material(Material* mat) {
        phong = mat->phong;
        ambientStrength = mat->ambientStrength;
        specularStrength = mat->specularStrength;
        reflectiveStrength = mat->reflectiveStrength;
        refractiveStrength = mat->refractiveStrength;
        indexOfRefraction = mat->indexOfRefraction;
    }
    virtual ~Material(){}

    float reflectiveStrength;
    float refractiveStrength;
    float indexOfRefraction;

    glm::vec3 getColor(glm::vec3 interPos, glm::vec3 normal, glm::vec3 viewPos, glm::vec2 uv, std::vector<Light*> lights, std::vector<bool> shadows) {

        glm::vec3 objectColor = getObjectColor(interPos);
        glm::vec3 final_light = glm::vec3(0.0f, 0.0f, 0.0f);
        for(int i = 0;i < lights.size();i++) {
            if(!shadows[i]) {
                Light* light = lights[i];
                glm::vec3 lightColor = light->getColor();


                glm::vec3 ambient = ambientStrength * lightColor;

                glm::vec3 norm = glm::normalize(normal);
                glm::vec3 lightDir = glm::normalize(light->getDirection(interPos));
                float diff = std::max(glm::dot(norm, lightDir), 0.0f);
                glm::vec3 diffuse = diff * lightColor;

                glm::vec3 viewDir = glm::normalize(viewPos - interPos);
                glm::vec3 reflectDir = glm::reflect(-lightDir, norm);
                float spec = pow(std::max(glm::dot(viewDir, reflectDir), 0.0f), phong);
                glm::vec3 specular = specularStrength * spec * lightColor;

                final_light += (ambient + diffuse + specular);
            }
        }
        return final_light * objectColor;
    }

    int getID() {
        return id;
    }
class Geometry* shape;
protected:
    int id;
private:
    virtual glm::vec3 getObjectColor(glm::vec2 uv) = 0;
    int phong;
    float ambientStrength;
    float specularStrength;


};

#endif
