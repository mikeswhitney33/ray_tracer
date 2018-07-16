#ifndef OCTREE_H
#define OCTREE_H

#include <vector>

#include <geometry.hpp>

class Octree {
public:
    Octree(std::vector<Geometry*> shapes) {
        lowExtent = shapes[0].getLowExtents();
        highExtent = shapes[0].getHighExtents();
        for(int i = 0;i < shapes.size();i++){
            Geometry* shape = shapes[i];
            glm::vec3 shape_low = shape->getLowExtents();
            glm::vec3 shape_high = shape->getHighExtents();
            for(int j = 0;j < 3;j++) {
                if(shape_low[j] < lowExtent[j]) {
                    lowExtent[j] = shape_low[j];
                }
                if(shape_high[j] > highExtnet[j]){
                    highExtent[j] = shape_high[j];
                }
            }
        }
    }

private:
    glm::vec3 lowExtent;
    glm::vec3 highExtent;
};

#endif
