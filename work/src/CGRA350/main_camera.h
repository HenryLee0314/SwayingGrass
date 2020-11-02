#ifndef MAIN_CAMERA_H
#define MAIN_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include "camera.h"

namespace CGRA350 {


class MainCamera
{
public:
    static MainCamera* getInstance();

    glm::mat4 GetViewMatrix() {
        if (_currectCamera) {
            return _currectCamera->GetViewMatrix();
        }
        return glm::mat4(1.0f);
    }

    glm::mat4 GetProjectionMatrix() {
        if (_currectCamera) {
            return _currectCamera->GetProjectionMatrix();
        }
        return glm::mat4(1.0f);
    }

    inline glm::vec3 getPosition() {
        if (_currectCamera) {
            return _currectCamera->getPosition();
        }
        return glm::vec3(0, 0, 0);
    }

    void setYaw(float yaw) {
        if (_currectCamera == &_mainCamera) {
            _currectCamera->setYaw(yaw);
        }
    }

    void setPitch(float pitch) {
        if (_currectCamera == &_mainCamera) {
            _currectCamera->setPitch(pitch);
        }
    }

    void setDistance(float distance) {
        if (_currectCamera == &_mainCamera) {
            _currectCamera->setDistance(distance);
        }
    }

private:
    MainCamera();
    virtual ~MainCamera();
    MainCamera(const MainCamera&);
    MainCamera& operator = (const MainCamera&);

private:
    static MainCamera* _instance;

    Camera _mainCamera;

    Camera* _currectCamera;
};

} // namespace CGRA350

#endif // MAIN_CAMERA_H