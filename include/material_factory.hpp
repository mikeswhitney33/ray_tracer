#ifndef MATERIAL_FACTORY_H
#define MATERIAL_FACTORY_H

#include <material.hpp>
#include <materials/color_material.hpp>
#include <materials/texture_material.hpp>


class MaterialFactory {
public:
    Material* create(Material* mat) {
        switch(mat->getID()) {
        case 0:
            return new ColorMaterial(mat);
            break;
        default:
            return new TextureMaterial(mat);
            break;
        }

    }
};

#endif
