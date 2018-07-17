#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include <geometry.hpp>
#include <glm/glm.hpp>
#include <ray.hpp>

/*
 * Class: BoundingBox
 * ------------------
 * A class that represents an axis aligned bounding box.
 */
class BoundingBox {
public:
    /*
     *  Constructor
     *  -----------
     *  Constructs a new bounding box.
     *
     * Parameters:
     *      vec3 vmin: the low extents of the bounding box.
     *      vec3 vmax: the high extents of the bounding box.
     */
    BoundingBox(const glm::vec3 &vmin, const glm::vec3 &vmax) {
        bounds[0] = vmin;
        bounds[1] = vmax;
    }
    /*
     *  Copy Constructor
     *  -----------
     *  Constructs a new bounding box using an already existing bounding box.
     *
     * Parameters:
     *      BoundingBox box: the bounding box we are copying.
     */
    BoundingBox(const BoundingBox &box) {
        bounds[0] = box.bounds[0];
        bounds[1] = box.bounds[1];
    }

    /*
     * Method: intersect
     * -------------------
     * checks to see if two bounding boxes intersect.
     *
     * Paramters:
     *      BoundingBox box: the bounding box we are doing an intersection test with.
     * Returns: true if the two bounding boxes intersect, false otherwise.
     */
    bool intersect(const BoundingBox &box) {
        for(int i = 0;i < 3;i++) {
            if(box.bounds[0][i] > bounds[1][i] || bounds[0][i] > box.bounds[1][i]) {
                return false;
            }
        }
        return true;
    }

    /*
     * Method: intersect
     * -------------------
     * checks to see if a ray intersects with the bounding box.
     *
     * Paramters:
     *      Ray r: the ray we are testing for intersection with.
     * Returns: true if the ray intersects the bounding box, false otherwise.
     */
    bool intersect(const Ray &r) {
        float tmin, tmax, tymin, tymax, tzmin, tzmax;
        tmin = (bounds[r.sign[0]].x - r.orig.x) * r.invdir.x;
        tmax = (bounds[1 - r.sign[0]].x - r.orig.x) * r.invdir.x;
        tymin = (bounds[r.sign[1]].y - r.orig.y) * r.invdir.y;
        tymax = (bounds[1 - r.sign[1]].y - r.orig.y) * r.invdir.y;

        if((tmin > tymax) || (tymin > tmax)) {
            return false;
        }
        if(tymin > tmin) {
            tmin = tymin;
        }
        if(tymax < tmax) {
            tmax = tymax;
        }

        tzmin = (bounds[r.sign[2]].z - r.orig.z) * r.invdir.z;
        tzmax = (bounds[1 - r.sign[2]].z - r.orig.z) * r.invdir.z;

        if((tmin > tzmax) || (tzmin > tmax)) {
            return false;
        }
        if(tzmin > tmin) {
            tmin = tzmin;
        }
        if(tzmax < tmax) {
            tmax = tzmax;
        }
        return true;
    }

    /*
     * Data Member: bounds
     * -------------------
     * the high and low extents of the bounding box.
     */
    glm::vec3 bounds[2];
};

#endif
