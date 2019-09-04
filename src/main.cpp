#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <limits>

#include "../headers/geometry/vec3.hpp"
#include "../headers/factories/material-factory.hpp"
#include "../headers/factories/shape-factory.hpp"


#define clamp(x) (x < 0 ? 0 : x > 1 ? 1 : x)
#define toInt(x) (int(pow(clamp(x), 1 / 2.2) * 255 + 0.5))
#define NUM_SHAPES 9


Shape* shapes[NUM_SHAPES] = {
    makeSphere(makeDiffuseMaterial(vec3(0.75, 0.25, 0.25), vec3()), vec3( 1e5+1,40.8,81.6), 1e5),
    makeSphere(makeDiffuseMaterial(vec3(0.25, 0.25, 0.75), vec3()), vec3(-1e5+99,40.8,81.6), 1e5),
    makeSphere(makeDiffuseMaterial(vec3(0.75, 0.75, 0.75), vec3()), vec3(50, 40.8, 1e5), 1e5),
    makeSphere(makeDiffuseMaterial(vec3(), vec3()), vec3(50, 40.8, -1e5+170), 1e5),
    makeSphere(makeDiffuseMaterial(vec3(0.75, 0.75, 0.75), vec3()), vec3(50, 1e5, 81.6), 1e5),
    makeSphere(makeDiffuseMaterial(vec3(0.75, 0.75, 0.75), vec3()), vec3(50, -1e5+81.6, 81.6), 1e5),
    makeSphere(makeSpecularMaterial(vec3(0.999, 0.999, 0.999), vec3()), vec3(27, 16.5, 47), 16.5),
    makeSphere(makeRefractiveMaterial(vec3(0.999, 0.999, 0.999), vec3()), vec3(73, 16.5, 78), 16.5),
    makeSphere(makeDiffuseMaterial(vec3(), vec3(12, 12, 12)), vec3(50, 681.6-0.27, 81.6), 600)
};

inline bool intersect(const vec3 &ro, const vec3 &rd, double &t, int &id) {
    double d;
    t = std::numeric_limits<double>::max();
    id = -1;
    for(int i=NUM_SHAPES;i--;) {
        d = shapes[i]->intersect(ro, rd);
        if(d && d < t){
            t=d;
            id=i;
        }
    }
    return id > -1;
}


vec3 radiance(const vec3 &ro, const vec3 &rd, int depth){
    double t;
    int id;
    if (!intersect(ro, rd, t, id)) {
        return vec3();
    }
    const Shape* obj = shapes[id];
    const Material* mat = obj->getMat();
    vec3 pt = ro + rd * t;
    vec3 n = obj->getNormal(rd, pt);
    vec3 nl = dot(n, rd) < 0 ? n : n * -1;
    vec3 f = mat->getColor();
    double p = max(f);
    if (++depth > 5) {
        if(drand48() < p) {
            f = f * (1 / p);
        }
        else {
            return mat->getEmission();
        }
    }
    return mat->getEmission() + f * mat->incoming(pt, rd, n, nl, depth, radiance);
}

int main(int argc, char *argv[]) {
    int screen_width = 1024;
    int screen_height = 768;
    int samps = 1;
    if(argc == 2) {
        samps = atoi(argv[1]) / 4;
    }
    vec3 camo(50, 52, 295.6);
    vec3 camd = vec3(0, -0.042612, -1).norm();
    vec3 cx = vec3(screen_width * 0.5135 / screen_height);
    vec3 cy = cross(cx, camd).norm() * .5135;
    vec3 r;
    vec3 *c = new vec3[screen_width * screen_height];
#pragma omp parallel for schedule(dynamic, 1) private(r)
    for (int screen_y = 0; screen_y < screen_height; screen_y++){
        fprintf(stderr, "\rRendering (%d spp) %5.2f%%", samps * 4, 100. * screen_y / (screen_height - 1));
        for (unsigned short screen_x = 0; screen_x < screen_width; screen_x++) {
            for (int sy=0, i=(screen_height - screen_y - 1) * screen_width + screen_x; sy<2; sy++) {
                for (int sx=0; sx<2; sx++, r=vec3()) {
                    for (int s=0; s<samps; s++) {
                        double r1 = 2.0 * drand48();
                        double dx = r1 < 1.0 ? sqrt(r1) - 1.0 : 1.0 - sqrt(2 - r1);
                        double r2 = 2 * drand48();
                        double dy = r2 < 1.0 ? sqrt(r2) - 1.0 : 1.0 - sqrt(2 - r2);
                        vec3 d = cx * (((sx + 0.5 + dx) / 2 + screen_x) / screen_width - 0.5) +
                                cy * (((sy + .5 + dy) / 2 + screen_y) / screen_height - 0.5) + camd;
                        r = r + radiance(camo + d * 140, d.norm(), 0) * (1.0 / samps);
                    }
                    c[i] = c[i] + vec3(clamp(r.x), clamp(r.y), clamp(r.z)) * 0.25;
                }
            }
        }
    }
    FILE *f = fopen("image.ppm", "w");
    fprintf(f, "P3\n%d %d\n%d\n", screen_width, screen_height, 255);
    for (int i = 0; i < screen_width * screen_height; i++) {
        fprintf(f,"%d %d %d ", toInt(c[i].x), toInt(c[i].y), toInt(c[i].z));
    }

    for(int i = 0;i < NUM_SHAPES;i++) {
        delete shapes[i];
    }
}
