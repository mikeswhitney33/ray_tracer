#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

/*
 * Class Camera
 * ------------
 * A class to represent a 3d camera.
 */
class Camera {
public:
    /*
     * Constructor
     * -----------
     * Sets up a new camera.
     *
     * Parameters:
     *      vec3 position: the current position of the camera.
     *      vec3 forward: the location that the camera is looking at.
     *      vec3 up: the up direction for the camera.
     */
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3 forward = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3 up = glm::vec3(0.0f, -1.0f, 0.0f)) {

        Position = position;
        glm::vec3 N = glm::normalize(forward - Position);
        glm::vec3 U = glm::normalize(glm::cross(up, N));
        glm::vec3 V = glm::normalize(glm::cross(N, U));

        float M[4][4] = {
            {U.x, V.x, N.x, forward.x},
            {U.y, V.y, N.y, forward.y},
            {U.z, V.z, N.z, forward.z},
            {0.0f, 0.0f, 0.0f, 1.0f}
        };
        memcpy(glm::value_ptr(vrc), M, sizeof(M));
    }

    /*
     * Data Member: Position
     * ---------------------
     * the current position of the camera.
     */
    glm::vec3 Position;

    /*
     * Data Member: vrc
     * ----------------
     * the view reference coordinates transformation matrix.  This matrix
     * tranforms points from screen coordinates to world coordinates.
     */
    glm::mat4 vrc;
};

#endif
