#include "main_camera.h"

namespace CGRA350 {

MainCamera* MainCamera::_instance = nullptr;

MainCamera* MainCamera::getInstance()
{
    if (_instance == nullptr) {
        _instance = new MainCamera();
    }

    return _instance;
}

MainCamera::MainCamera()
    : _mainCamera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f))
    , _currectCamera(&_mainCamera)
{

}

MainCamera::~MainCamera()
{
    _currectCamera = nullptr;
}

} // namespace CGRA350