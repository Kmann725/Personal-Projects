#pragma once

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CAMERA_MOVE_DIRECTION {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 1.5f;
const float SENSITIVITY = 0.1f;

class Camera {
public:
    float yaw, pitch;
    float movementSpeed, mouseSensitivity;
    glm::vec3 pos, front, up, right, worldUp;

    Camera(glm::vec3 pos, glm::vec3 up);

    glm::mat4 GetViewMatrix();
    void ProcessKeyboard(CAMERA_MOVE_DIRECTION direction, float dt);
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

private:
    void updateCameraVectors();
};

