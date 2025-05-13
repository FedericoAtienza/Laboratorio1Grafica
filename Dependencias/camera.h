#include "variables.h"
#include "worm_variable.h"
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#ifdef __APPLE__ // macOS
#include <OpenGL/glu.h>
#include <SDL2/SDL.h>
#else
#include "SDL.h"
#include <GL/glu.h>
#endif

#ifndef CAMERA_H
#define CAMERA_H

enum CameraType {
    FIRST_PERSON,
    FORTH_PERSON,
    FREE_CAMERA
};

class Camera {
  private:
    float yaw;   // Horizontal angle
    float pitch; // Vertical angle
    float sensitivity = 0.1f;
    float cameraSpeedBase = 1.0f;
    float cameraPosX, cameraPosY, cameraPosZ;
    float cameraFrontX, cameraFrontY, cameraFrontZ;
    float cameraUpX = 0.0f, cameraUpY = 1.0f, cameraUpZ = 0.0f;
    CameraType cameraType;

    // Flat forward vector
    float flatFrontX = 0.0f;
    float flatFrontZ = 1.0f;
    // Flat right vector
    float rightX = flatFrontZ;
    float rightZ = -flatFrontX;

    void updateCameraVectors();
    void updateViewVectors();
    void setCameraTypeForthPerson();
    void setCameraTypeFreeCamera();
    void setCameraTypeFirstPerson();

  public:
    Camera();
    void setViewMatrix();
    void handleMouseMotion(float xrel, float yrel);
    void handleMovementKeys();
    void changeCameraType();
};

Camera::Camera() {
    setCameraTypeFreeCamera();
}

void Camera::updateCameraVectors() {
    float frontX = cosf(glm::radians(yaw)) * cosf(glm::radians(pitch));
    float frontY = sinf(glm::radians(pitch));
    float frontZ = sinf(glm::radians(yaw)) * cosf(glm::radians(pitch));
    float magnitude = sqrt(frontX * frontX + frontY * frontY + frontZ * frontZ);
    cameraFrontX = frontX / magnitude;
    cameraFrontY = frontY / magnitude;
    cameraFrontZ = frontZ / magnitude;
}

void Camera::setViewMatrix() {
    gluLookAt(cameraPosX, cameraPosY, cameraPosZ, cameraPosX + cameraFrontX, cameraPosY + cameraFrontY, cameraPosZ + cameraFrontZ, cameraUpX, cameraUpY, cameraUpZ);
}

void Camera::handleMouseMotion(float xrel, float yrel) {
    if (cameraType == FORTH_PERSON)
        return;

    yaw += xrel * sensitivity;
    pitch -= yrel * sensitivity;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    updateCameraVectors();
}

void Camera::updateViewVectors() {
    // Calculate flat forward vector
    flatFrontX = cameraFrontX;
    flatFrontZ = cameraFrontZ;

    // Normalize the flat forward vector
    float magnitude = sqrt(flatFrontX * flatFrontX + flatFrontZ * flatFrontZ);
    flatFrontX /= magnitude;
    flatFrontZ /= magnitude;

    // Recalculate right vector
    rightX = flatFrontZ;
    rightZ = -flatFrontX;
}

void Camera::handleMovementKeys() {
    float cameraSpeed = cameraSpeedBase * deltaTime * game_speed;
    const Uint8* state = SDL_GetKeyboardState(NULL);

    if (cameraType == FIRST_PERSON) {
        Point wormHead = worm.get_head();
        cameraPosX = wormHead.x;
        cameraPosY = wormHead.y + 1.0f;
    }
    if (cameraType == FORTH_PERSON) {
        // Handle movement
        if (state[SDL_SCANCODE_W]) {
            cameraPosY += cameraSpeed;
        }
        if (state[SDL_SCANCODE_S]) {
            cameraPosY -= cameraSpeed;
        }
        if (state[SDL_SCANCODE_A]) {
            cameraPosX -= cameraSpeed;
        }
        if (state[SDL_SCANCODE_D]) {
            cameraPosX += cameraSpeed;
        }
    }
    if (cameraType == FREE_CAMERA) {
        updateViewVectors();

        // Handle movement
        if (state[SDL_SCANCODE_W]) {
            cameraPosX += cameraSpeed * flatFrontX;
            cameraPosZ += cameraSpeed * flatFrontZ;
        }
        if (state[SDL_SCANCODE_S]) {
            cameraPosX -= cameraSpeed * flatFrontX;
            cameraPosZ -= cameraSpeed * flatFrontZ;
        }
        if (state[SDL_SCANCODE_A]) {
            cameraPosX += cameraSpeed * rightX;
            cameraPosZ += cameraSpeed * rightZ;
        }
        if (state[SDL_SCANCODE_D]) {
            cameraPosX -= cameraSpeed * rightX;
            cameraPosZ -= cameraSpeed * rightZ;
        }
        if (state[SDL_SCANCODE_SPACE]) {
            cameraPosY += cameraSpeed;
        }
        if (state[SDL_SCANCODE_LSHIFT]) {
            cameraPosY -= cameraSpeed;
        }
    }
}

void Camera::changeCameraType() {
    if (cameraType == FIRST_PERSON) {
        setCameraTypeForthPerson();
    } else if (cameraType == FORTH_PERSON) {
        setCameraTypeFreeCamera();
    } else if (cameraType == FREE_CAMERA) {
        setCameraTypeFirstPerson();
    }
}

void Camera::setCameraTypeForthPerson() {
    cameraType = FORTH_PERSON;

    SDL_SetRelativeMouseMode(SDL_FALSE);

    yaw = -90.0f;
    pitch = 0.0f;

    cameraPosX = 0.0f;
    cameraPosY = 2.0f;
    cameraPosZ = 10.0f;

    cameraFrontX = 0.0f;
    cameraFrontY = 0.0f;
    cameraFrontZ = -1.0f;
}

void Camera::setCameraTypeFreeCamera() {
    cameraType = FREE_CAMERA;

    SDL_SetRelativeMouseMode(SDL_TRUE);

    yaw = -90.0f;
    pitch = 0.0f;

    cameraPosX = 0.0f;
    cameraPosY = 2.0f;
    cameraPosZ = 10.0f;

    cameraFrontX = 0.0f;
    cameraFrontY = 0.0f;
    cameraFrontZ = -1.0f;
}

void Camera::setCameraTypeFirstPerson() {
    cameraType = FIRST_PERSON;

    SDL_SetRelativeMouseMode(SDL_TRUE);

    yaw = -90.0f;
    pitch = 0.0f;

    cameraPosX = 0.0f;
    cameraPosY = 1.0f;
    cameraPosZ = 0.0f;

    cameraFrontX = 1.0f;
    cameraFrontY = 0.0f;
    cameraFrontZ = 0.0f;
}

#endif