#ifndef TRIANGLE_H
#define TRIANGLE_H


#include <geometry.hpp>
#include <glm/gtx/component_wise.hpp>

class Triangle : public Geometry {
public:
    Triangle(Material* mat, glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec2 a_uv=glm::vec2(0.5f, 1.0f), glm::vec2 b_uv=glm::vec2(0.0f, 0.0f), glm::vec2 c_uv=glm::vec2(1.0f, 0.0f)): Geometry(mat) {
        A = a;
        B = b;
        C = c;
    }

    virtual ~Triangle() {
        std::cout << "Destructing Triangle!" << std::endl;
    }

    bool intersect(glm::vec3 r0, glm::vec3 rd, glm::vec3 &normal, float &t) {
        glm::vec3 u = B - A;
        glm::vec3 v = C - A;

        normal = glm::cross(u, v);
        float prd = glm::dot(normal, rd);
        if(prd == 0) {
            return false;
        }

        float d = -(glm::dot(normal, A));
        t = -((glm::dot(normal, r0) + d) / prd);
        if(t <= 0) {
            return false;
        }
        if(prd >= 0) {
            normal = -normal;
        }

        glm::vec3 inter = r0 + t * rd;

        float uu, uv, vv, wu, wv, D;
        uu = glm::dot(u, u);
        uv = glm::dot(u, v);
        vv = glm::dot(v, v);
        glm::vec3 w = inter - A;
        wu = glm::dot(w, u);
        wv = glm::dot(w, v);
        D = uv * uv - uu * vv;

        float s, tmp;
        s = (uv * wv - vv * wu) / D;
        if(s < 0 || s > 1) {
            return false;
        }
        tmp = (uv * wu - uu * wv) / D;
        if(tmp < 0 || (s + tmp) > 1) {
            return false;
        }

        return true;
    }

    glm::vec2 getUV(glm::vec3 pt) {
        return glm::vec2(0.0f, 0.0f);
    }
private:
    glm::vec3 A, B, C;
};

#endif