#include <iostream>
#include <fstream>
#include <vector>
#include <limits>

#include "../geometry/vec3.hpp"
#include "../geometry/shapes.hpp"

#define deg2rad(deg) deg * M_PI / 180.0f
#define NUM_SHAPES 17

std::vector<Shape*> shapes(NUM_SHAPES);

void initShapes() {
    // back
    shapes[0] = new Triangle(vec3(), vec3(0.75, 0.75, 0.75), DIFF, vec3(-500, -500, 500), vec3(-500, 500, 500), vec3(500, 500, 500));
    shapes[1] = new Triangle(vec3(), vec3(0.75, 0.75, 0.75), DIFF, vec3(-500, -500, 500), vec3(500, -500, 500), vec3(500, 500, 500));
    //front
    shapes[2] = new Triangle(vec3(), vec3(0.25, 0.25, 0.25), DIFF, vec3(-500, -500, -500), vec3(-500, 500, -500), vec3(500, 500, -500));
    shapes[3] = new Triangle(vec3(), vec3(0.25, 0.25, 0.25), DIFF, vec3(-500, -500, -500), vec3(500, -500, -500), vec3(500, 500, -500));
    // left
    shapes[4] = new Triangle(vec3(), vec3(0.75, 0.25, 0.25), DIFF, vec3(-500, -500, -500), vec3(-500, 500, -500), vec3(-500, 500, 500));
    shapes[5] = new Triangle(vec3(), vec3(0.75, 0.25, 0.25), DIFF, vec3(-500, -500, -500), vec3(-500, -500, 500), vec3(-500, 500, 500));
    // right
    shapes[6] = new Triangle(vec3(), vec3(0.25, 0.25, 0.75), DIFF, vec3(500, -500, -500), vec3(500, 500, -500), vec3(500, 500, 500));
    shapes[7] = new Triangle(vec3(), vec3(0.25, 0.25, 0.75), DIFF, vec3(500, -500, -500), vec3(500, -500, 500), vec3(500, 500, 500));
    // top
    shapes[8] = new Triangle(vec3(), vec3(0.75, 0.75, 0.75), DIFF, vec3(-500, 500, -500), vec3(500, 500, -500), vec3(500, 500, 500));
    shapes[9] = new Triangle(vec3(), vec3(0.75, 0.75, 0.75), DIFF, vec3(-500, 500, -500), vec3(-500, 500, 500), vec3(500, 500, 500));
    // bottom
    shapes[10] = new Triangle(vec3(), vec3(0.75, 0.75, 0.75), DIFF, vec3(-500, -500, -500), vec3(500, -500, -500), vec3(500, -500, 500));
    shapes[11] = new Triangle(vec3(), vec3(0.75, 0.75, 0.75), DIFF, vec3(-500, -500, -500), vec3(-500, -500, 500), vec3(500, -500, 500));
    // light
    shapes[12] = new Triangle(vec3(12, 12, 12), vec3(), DIFF, vec3(-75, 490, 150), vec3(75, 490, 150), vec3(75, 490, 300));
    shapes[13] = new Triangle(vec3(12, 12, 12), vec3(), DIFF, vec3(-75, 490, 150), vec3(-75, 490, 300), vec3(75, 490, 300));
    // Mirror
    shapes[14] = new Sphere(vec3(), vec3(0.999, 0.999, 0.999), SPEC, vec3(-350, -500 + 100, 350), 100);
    // Glass
    shapes[15] = new Sphere(vec3(), vec3(0.999, 0.999, 0.999), SPEC, vec3(350, -500 + 100, 300), 100);
    shapes[16] = new Sphere(vec3(), vec3(0.75, 0.25, 0.25), DIFF, vec3(0, 0, -1000), 500);
}

void freeShapes() {
    for(auto& shape : shapes) {
        delete shape;
    }
}

int intersect(const vec3 &ray_orig, const vec3 &ray_dir, float &min_t) {
    int shapeId = -1;
    for(int i = 0;i < shapes.size();i++) {
        Shape* shape = shapes[i];
        if(shape->intersect(ray_orig, ray_dir, min_t)) {
            shapeId = i;
        }
    }
    return shapeId;
}

void createCoordinateSystem(const vec3 &N, vec3 &Nt, vec3 &Nb) {
    if(fabs(N.x) > fabs(N.y)) {
        Nt = vec3(N.z, 0, -N.x) / sqrtf(N.x * N.x + N.z * N.z);
    }
    else {
        Nt = vec3(0, -N.z, N.y) / sqrtf(N.y * N.y + N.z * N.z);
    }
    Nb = cross(N, Nt);
}

vec3 uniformSampleHemisphere(const float &r1, const float &r2) {
    float sinTheta = sqrtf(1 - r1 * r1);
    float phi = 2 * M_PI * r2;
    float x = sinTheta * cosf(phi);
    float z = sinTheta * sinf(phi);
    return vec3(x, r1, z);
}


vec3 trace_ray(const vec3 &ray_orig, const vec3 &ray_dir, int depth) {
    if(depth > 5) {
        return vec3();
    }
    float t = std::numeric_limits<float>::max();
    int shapeId = intersect(ray_orig, ray_dir, t);
    if(shapeId < 0) return vec3();
    Shape* shape = shapes[shapeId];

    vec3 pt = ray_orig + ray_dir * t;
    vec3 normal = shape->getNormal(ray_dir, pt);
    vec3 nl = dot(normal, ray_dir) < 0 ? normal : -1.0f * normal;
    vec3 color = shape->getColor();
    float p = max(color);
    if(++depth > 5) {
        if(drand48() < p) {
            color *= (1.0f/p);
        }
        else {
            return shape->getEmission();
        }
    }
    if(shape->getMat() == DIFF) {
        vec3 sample;
        {
            float r1 = drand48(), r2 = drand48();
            float sinTheta = sqrtf(1 - r1 * r2);
            float phi = 2 * M_PI * r2;
            float x = sinTheta * cosf(phi);
            float z = sinTheta * sinf(phi);
            sample = vec3(x, r1, z);
        }
        vec3 Nt, Nb;
        {
            if(fabs(normal.x) > fabs(normal.y)) {
                Nt = vec3(normal.z, 0, -normal.x) / sqrtf(normal.x * normal.x + normal.z * normal.z);
            }
            else {
                Nt = vec3(0, -normal.z, normal.y) / sqrtf(normal.y * normal.y + normal.z * normal.z);
            }
            Nb = cross(normal, Nt);
        }
        vec3 new_dir = vec3(
            sample.x * Nb.x + sample.y * normal.x + sample.z * Nt.x,
            sample.x * Nb.y + sample.y * normal.y + sample.z * Nt.y,
            sample.x * Nb.z + sample.y * normal.z + sample.z * Nt.z);
        new_dir -= pt;
        return shape->getEmission() + color * trace_ray(pt + new_dir * EPS, norm(new_dir), depth);

    // virtual glm::vec3 diffuse(const glm::vec3 &pt, glm::vec3 rd, const glm::vec3 &normal, const int  &recursions, const float &eta1, const glm::vec3 &Nt, const glm::vec3 &Nb, const float &r1, const float &r2) {
    //     glm::vec3 sam = uniform_sample_hemisphere(r1, r2);

    //     rd = glm::vec3(
    //         sam.x * Nb.x + sam.y * normal.x + sam.z * Nt.x,
    //         sam.x * Nb.y + sam.y * normal.y + sam.z * Nt.y,
    //         sam.x * Nb.z + sam.y * normal.z + sam.z * Nt.z);
    //     return sample(Ray(DIFFUSE, pt + rd * SMALL_NUM, rd), recursions + 1, eta1);
    // }
        // float r1 = 2 * M_PI * drand48();
        // float r2 = drand48();
        // float r2s = sqrtf(r2);
        // vec3 w = nl;
        // vec3 u = norm(cross(fabs(w.x) > .1 ? vec3(0, 1) : vec3(1), w));
        // vec3 v = cross(w, u);
        // vec3 d = norm(u * cosf(r1) * r2s + v * sinf(r1) * r2s + w * sqrtf(1 - r2));
        // return shape->getEmission() + color * trace_ray(pt, d, depth);
    }
    else if(shape->getMat() == SPEC) {
        return shape->getEmission() + color * trace_ray(pt, reflect(ray_dir, normal), depth);
    }

    return vec3();
}

int main(int argc, char** argv) {
    initShapes();
    int width = 300, height = 200;
    int num_samples = 20;
    float fov = 90.0f;
    float scale = tanf(deg2rad(fov * 0.5f));
    float aspect = static_cast<float>(width) / static_cast<float>(height);
    std::ofstream f("out.ppm");
    f << "P3\n" << width << " " << height << "\n255\n";
    vec3 ray_orig(0, 0, -450);
    for(int screen_y = 0;screen_y < height;screen_y++) {
        std::fprintf(stderr, "\r (%d SPP) %5.2f%%  ", num_samples, 100.0f * ((screen_y+1)/static_cast<float>(height)));
        // std::cout << "\r  (" << num_samples << " SPP) " <<  << "      " << std::flush;
        for(int screen_x = 0;screen_x < width;screen_x++) {
            float x = (2.0f * (static_cast<float>(screen_x) + 0.5f) / static_cast<float>(width) - 1.0f) * scale * aspect;
            float y = (1 - 2.0f * (static_cast<float>(screen_y) + 0.5f) / static_cast<float>(height)) * scale;
            vec3 ray_dir = norm(vec3(x, y, ray_orig.z + 1) - ray_orig);
            vec3 color;
            for(int i = 0;i < num_samples;i++) {
                color += (trace_ray(ray_orig, ray_dir, 0) / static_cast<float>(num_samples));
            }
            f << toPixel(color);
        }
    }
    std::cout << std::endl;
    f.close();
    freeShapes();
    return 0;
}