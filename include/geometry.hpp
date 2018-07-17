#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <material.hpp>
#include <bounding_box.hpp>
#include <ray.hpp>

/*
 * Class: Geometry
 * ---------------
 * A base class reprenting basic geometry in a scene.
 */
class Geometry {
public:
    /*
     * Cosntructor
     * -----------
     * Constructs a new piece of geometry.
     *
     * Paramters:
     *      Material mat: the material of the geometry we are making.
     */
    Geometry(Material* mat):bounding_box(glm::vec3(), glm::vec3()) {
        material = mat;
    }

    /*
     * Destructor
     * ----------
     * Deallocates memory for allocated data members.  In this case the material
     * needs to be deallocated.
     */
    virtual ~Geometry(){
        delete material;
    }

    /*
     * Method: intersect
     * -----------------
     * Tests for intersection between the shape and the ray. (To be implemented by
     * derived classes).
     *
     * Parameters:
     *      Ray ray: the ray we are testing for intersection with.
     *      vec3 normal: the normal that will be set during the intersection test.
     *      float t: the t value, meaning how far away the shape is.
     *      vec3 uv: the texture coordinates calculated from the intersection point.
     * Returns: true if there is an intersection, false otherwise.
     */
    virtual bool intersect(
        const Ray &ray, glm::vec3 &normal, float &t, glm::vec2 &uv) = 0;

    /*
     * Method: intersect
     * -----------------
     * Tests for intersection between the shape and another bounding box.
     *
     * Paramters:
     *      BoundingBox box: the bounding box we are testing for intersection with.
     * Returns: true if there is an intersection, false otherwise.
     */
    bool intersect(const BoundingBox &box) {
        return bounding_box.intersect(box);
    }

    /*
     * Data Member: material
     * ---------------------
     * The material assigned to this piece of geometry.
     */
    Material* material;

    /*
     * Data Member: bounding_box
     * -------------------------
     * The axis aligned bounding box enclosing this shape.
     */
    BoundingBox bounding_box;
private:
    /*
     * Method: setExtents
     * ------------------
     * initializes the extents of the shape's bounding box (to be implemented by the
     * derived classes).
     */
    virtual void setExtents() = 0;
};

#endif
