#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include <glm/glm.hpp>
#include <ray.hpp>

class BoundingBox {
public:
    BoundingBox() {}
    BoundingBox(const glm::vec3 &vmin, const glm::vec3 &vmax) {
        bounds[0] = vmin;
        bounds[1] = vmax;
    }
    glm::vec3 bounds[3];

    bool intersect(const BoundingBox &box) {
        for(int i = 0;i < 3;i++) {
            if(box.bounds[0][i] > bounds[1][i] || bounds[0][i] > box.bounds[1][i]) {
                return false;
            }
        }
        return true;
    }

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

    friend std::ostream& operator<<(std::ostream& os, const BoundingBox b) {
        os << "Low: (" << b.bounds[0].x << "," << b.bounds[0].y << "," << b.bounds[0].z << ") -- High: (" << b.bounds[1].x << "," << b.bounds[1].y << "," << b.bounds[1].z << ")";
        return os;
    }
};
// class BoundingBox {
// public:
//     BoundingBox(glm::vec3 low, glm::vec3 high) {
//         lowExtent = low;
//         highExtent = high;
//     }
//
//     glm::vec3 lowExtent;
//     glm::vec3 highExtent;
//
//     friend std::ostream& operator<<(std::ostream& os, const BoundingBox b) {
//         os << "Low: (" << b.lowExtent.x << "," << b.lowExtent.y << "," << b.lowExtent.z << ") -- High: (" << b.highExtent.x << "," << b.highExtent.y << "," << b.highExtent.z << ")";
//         return os;
//     }
//
//     bool intersect(BoundingBox* box) {
//         for(int i = 0;i < 3;i++) {
//             if(box->lowExtent[i] > highExtent[i] || lowExtent[i] > box->highExtent[i]) {
//                 return false;
//             }
//         }
//         return true;
//     }
//
//     bool intersect(glm::vec3 r0, glm::vec3 rd, float &t_near){
//         t_near = -9999999.0f;
//         float t_far = 9999999.0f;
//         if(!checkShapeBoundingVolumePlane(lowExtent.x, highExtent.x, r0.x, rd.x, t_near, t_far)) {
//             return false;
//         }
//         if(!checkShapeBoundingVolumePlane(lowExtent.y, highExtent.y, r0.y, rd.y, t_near, t_far)) {
//             return false;
//         }
//         if(!checkShapeBoundingVolumePlane(lowExtent.z, highExtent.z, r0.z, rd.z, t_near, t_far)) {
//             return false;
//         }
//         return true;
//     }
// private:
//
//     bool checkShapeBoundingVolumePlane(float xl, float xh, float x0, float xd, float& t_near, float& t_far){
//         if(xd == 0) {
//           if(x0 < xl || x0 > xh) {
//             return false;
//           }
//         }
//         else {
//           float t1, t2;
//           t1 = (xl - x0)/xd;
//           t2 = (xh - x0)/xd;
//           if(t1 > t2) {
//             float tmp = t1;
//             t1 = t2;
//             t2 = tmp;
//           }
//           if(t1 > t_near) {
//             t_near = t1;
//           }
//           if(t2 < t_far){
//             t_far = t2;
//           }
//           if(t_near > t_far) {
//             return false;
//           }
//           if(t_far < 0) {
//             return false;
//           }
//         }
//         return true;
//     }
// };

#endif
