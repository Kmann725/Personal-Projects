#include "Camera.h"

Camera::Camera(glm::vec3 pos, glm::vec3 up) : pos(pos), up(up), yaw(YAW), pitch(PITCH), front(glm::vec3(0.0f, 0.0f, -1.0f)), worldUp(up), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY) {

}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(pos, pos + front, up);
}

void Camera::ProcessKeyboard(CAMERA_MOVE_DIRECTION direction, float dt) {
    float velocity = movementSpeed * dt;
    if (direction == FORWARD)
        pos += front * velocity;
    if (direction == BACKWARD)
        pos -= front * velocity;
    if (direction == LEFT)
        pos -= right * velocity;
    if (direction == RIGHT)
        pos += right * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) {
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (constrainPitch)
    {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::updateCameraVectors() {
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = -sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);
    
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}
