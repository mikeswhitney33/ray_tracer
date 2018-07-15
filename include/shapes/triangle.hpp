#ifndef TRIANGLE_H
#define TRIANGLE_H


#include <geometry.hpp>
#include <glm/gtx/component_wise.hpp>

class Triangle : public Geometry {
public:
    Triangle(Material* mat, glm::vec3 a, glm::vec3 b, glm::vec3 c)
        :Geometry(mat) {
        setVerts(a, b, c);
        hasVertexNormals = false;
        hasTextureUV = false;
        setSurfaceNormal();
        setDenom();
        setExtents();
    }
    Triangle(Material* mat, glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 an, glm::vec3 bn, glm::vec3 cn)
        :Geometry(mat) {
        setVerts(a, b, c);
        setNormals(an, bn, cn);
        hasVertexNormals = true;
        hasTextureUV = false;
        setSurfaceNormal();
        setDenom();
        setExtents();
    }
    Triangle(Material* mat, glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec2 at, glm::vec2 bt, glm::vec2 ct)
        :Geometry(mat) {
        setVerts(a, b, c);
        setTextureUV(at, bt, ct);
        hasTextureUV = true;
        hasVertexNormals = false;
        setSurfaceNormal();
        setDenom();
        setExtents();
    }

    Triangle(Material* mat, glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 an, glm::vec3 bn, glm::vec3 cn, glm::vec2 at, glm::vec2 bt, glm::vec2 ct)
        :Geometry(mat) {
        setVerts(a, b, c);
        setNormals(an, bn, cn);
        setTextureUV(at, bt, ct);
        hasTextureUV = true;
        hasVertexNormals = true;
        setSurfaceNormal();
        setDenom();
        setExtents();
    }

    virtual ~Triangle() {}

    bool intersect(glm::vec3 r0, glm::vec3 rd, glm::vec3 &normal, float &t) {
        if(!inBounds(r0, rd)) {
            return false;
        }
        glm::vec3 AB = B - A;
        glm::vec3 AC = C - A;
        glm::vec3 pvec = glm::cross(rd, AC);
        float det = glm::dot(AB, pvec);

        if(fabs(det) < 1e-8f){
            return false;
        }
        float invDet = 1.0f / det;
        glm::vec3 tvec = r0 - A;
        // float u, v;
        u = glm::dot(tvec, pvec) * invDet;
        if(u < 0 || u > 1) {
            return false;
        }
        glm::vec3 qvec = glm::cross(tvec, AB);
        v = glm::dot(rd, qvec) * invDet;
        if(v < 0 || u + v > 1) {
            return false;
        }

        t = glm::dot(AC, qvec) * invDet;
        // std::cout << "T: " << t << std::endl;
        if(t < 0) {
                return false;
        }


        normal = glm::cross(AB, AC);
        if(glm::dot(rd, normal) >= 0) {
            normal = glm::cross(AC, AB);
        }

        return true;
    }

    glm::vec2 getUV(glm::vec3 pt) {
        return Auv * u + Buv * v + Cuv * (1 - u - v);
        // glm::vec3 f1 = A - pt;
        // glm::vec3 f2 = B - pt;
        // glm::vec3 f3 = C - pt;
        //
        // float a = glm::length(glm::cross(A - B, A - C));
        // float a1 = glm::length(glm::cross(f2, f3)) / a;
        // float a2 = glm::length(glm::cross(f3, f1)) / a;
        // float a3 = glm::length(glm::cross(f1, f2)) / a;
        // return Auv * a1 + Buv * a2 + Cuv * a3;
        // return glm::vec2(0.0f, 0.0f);
    }
private:
    void setVerts(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
        A = a;B = b;C = c;
    }
    void setNormals(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
        An = a;Bn = b;Cn = c;
    }
    void setTextureUV(glm::vec2 a, glm::vec2 b, glm::vec2 c) {
        Auv = a;Buv = b;Cuv = c;
    }
    void setSurfaceNormal() {
        surfaceNormal = glm::cross(B - A, C - A);
    }
    void setDenom() {
        denom = glm::dot(surfaceNormal, surfaceNormal);
    }

    void setExtents() {
        lowExtents = A;
        highExtents = A;
        if(B.x < lowExtents.x) lowExtents.x = B.x;
        if(B.y < lowExtents.y) lowExtents.y = B.y;
        if(B.z < lowExtents.z) lowExtents.z = B.z;

        if(C.x < lowExtents.x) lowExtents.x = C.x;
        if(C.y < lowExtents.y) lowExtents.y = C.y;
        if(C.z < lowExtents.z) lowExtents.z = C.z;

        if(B.x > highExtents.x) highExtents.x = B.x;
        if(B.y > highExtents.y) highExtents.y = B.y;
        if(B.z > highExtents.z) highExtents.z = B.z;

        if(C.x > highExtents.x) highExtents.x = C.x;
        if(C.y > highExtents.y) highExtents.y = C.y;
        if(C.z > highExtents.z) highExtents.z = C.z;

    }
    glm::vec3 A, B, C;
    bool hasVertexNormals;
    glm::vec3 An, Bn, Cn;
    bool hasTextureUV;
    glm::vec2 Auv, Buv, Cuv;

    glm::vec3 surfaceNormal;
    float denom;

    float u, v;
};

#endif
