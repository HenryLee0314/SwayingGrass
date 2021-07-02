#include "light.h"

#include "cgra/cgra_gui.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace CGRA350 {

Light* Light::_instance = nullptr;

Light* Light::getInstance()
{
	if (_instance == nullptr) {
		_instance = new Light();
	}

	return _instance;
}

Light::Light()
	: _position(0.01, 20.0, 0.01)
	, _nearPlane(0.01f)
	, _farPlane(1000.0f)
{

}

Light::~Light()
{

}

glm::mat4 Light::getProjectionMatrix()
{
	// return glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, _nearPlane, _farPlane);
	return glm::perspective(1.f, float(3000 /*width*/) / 3000 /*height*/, 0.1f, 1000.f);
}

glm::mat4 Light::getViewMatirx()
{
	return glm::lookAt(_position, glm::vec3(0.0f, -1000.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Light::renderGUI()
{
	float data[3] = {_position.x, _position.y, _position.z};
	ImGui::Text("Light Position");
	ImGui::SliderFloat(" X", &data[0], -5.0f, 5.0f);
	ImGui::SliderFloat(" Y", &data[1], 0.0f, 30.0f);
	ImGui::SliderFloat(" Z", &data[2], -5.0f, 5.0f);
	_position.x = data[0];
	_position.y = data[1];
	_position.z = data[2];
}

} // namespace CGRA350