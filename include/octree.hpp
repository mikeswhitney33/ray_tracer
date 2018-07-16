#ifndef OCTREE_H
#define OCTREE_H

#include <vector>

#include <geometry.hpp>

struct GeometryNode {
    Geometry* shape;
    int index;
}

class OctreeNode {
public:
    OctreeNode(){}
    virtual ~OctreeNode(){}
    virtual bool isLeaf() = 0;
    virtual OctreeNode* add_shape(Geometry* shape, int index) = 0;
    virtual bool intersect(glm::vec3 r0, glm::vec3 rd, float &t, glm::vec3 &normal, glm::vec2 &uv, int &shape_index) = 0;
};

class OctreeBranch : public OctreeNode {
public:
    virtual bool isLeaf() {return false;}
    virtual OctreeNode* add_shape(Geometry* shape, int index) {
        return this;
    }

private:
    std::vector<OctreeNode*> children;
};

class OctreeLeaf : public OctreeNode {
    virtual bool isLeaf() {return true;}
    virtual OctreeNode* add_shape(Geometry* shape, int index) {
        return this;
    }
};

class Octree {
public:
    Octree(){

    }
    ~Octree(){}
    void add_shape(Geometry* shape, int index) {
        root = root->add_shape(shape, index);
    }

    bool intersect(glm::vec3 r0, glm::vec3 rd, float &t, glm::vec3 &normal, glm::vec2 &uv, int &shape_index) {
        return root->intersect(r0, rd, t, normal, uv, shape_index);
    }

private:
    OctreeNode* root;
};

#endif
