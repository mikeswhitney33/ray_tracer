#ifndef LIGHT_MATERIAL_H
#define LIGHT_MATERIAL_H

class LightMaterial : public Material {
public:
    //    Material(const int &n, const float &ka, const float &ks, const float &k_reflect=0.0f, const float &k_refract=0.0f, const float &i_refract=1.003) {
    LightMaterial(const glm::vec3 c):color(c), Material(0, 0, 0, 0, 0, 0){}
    LightMaterial(LightMaterial* mat):color(mat->color), Material(0, 0, 0, 0, 0, 0) {}
    virtual ~LightMaterial(){}

    virtual Material* copy() {
        return new LightMaterial(this);
    }
    virtual glm::vec3 getObjectColor(const glm::vec2 &uv) const {
        return color;
    }
    const glm::vec3 color;
};

#endif
