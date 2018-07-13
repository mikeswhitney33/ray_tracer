#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

enum CameraMovement {
    FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera {
public:
    glm::vec3 Position;
    glm::vec3 Forward;
    glm::vec3 Up;
    glm::mat4 vrc;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3 forward = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, -1.0f, 0.0f)) {
        Position = position;
        Forward = forward;
        Up = up;
        glm::vec3 N = glm::normalize(Forward - Position);
        glm::vec3 U = glm::normalize(glm::cross(Up, N));
        glm::vec3 V = glm::normalize(glm::cross(N, U));

        float M[4][4] = {
            {U.x, V.x, N.x, Forward.x},
            {U.y, V.y, N.y, Forward.y},
            {U.z, V.z, N.z, Forward.z},
            {0.0f, 0.0f, 0.0f, 1.0f}
        };
        memcpy(glm::value_ptr(vrc), M, sizeof(M));
    }

    virtual ~Camera() {}
};

#endif
