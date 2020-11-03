#include "shader_grass.h"

#include "opengl.hpp"

#include "cgra/cgra_gui.hpp"

#include "main_camera.h"
#include "light.h"

namespace CGRA350 {

GrassShader* GrassShader::_instance = nullptr;

GrassShader* GrassShader::getInstance()
{
	if (_instance == nullptr) {
		_instance = new GrassShader(CGRA_SRCDIR "/res/shaders/vertexShader/grass.vs", CGRA_SRCDIR "/res/shaders/fragmentShader/grass.fs", CGRA_SRCDIR "/res/shaders/geometryShader/grass.gs", CGRA_SRCDIR "/res/shaders/tessellationControlShader/grass.tcs", CGRA_SRCDIR "/res/shaders/tessellationEvaluationShader/grass.tes");
	}

	return _instance;
}

GrassShader::GrassShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath, const char* tessellationControlPath, const char* tessellationEvaluationPath)
	: Shader(vertexPath, fragmentPath, geometryPath, tessellationControlPath, tessellationEvaluationPath)
	, _ambientStrength(0.5)
	, _diffuseStrength(1)
	, _specularStrength(0.5)
	, _specularPower(5)
{

}

GrassShader::~GrassShader()
{

}

void GrassShader::use()
{
	Shader::use();

	glm::mat4 model = glm::mat4(1.0);

	Shader::setMat4("model", model);
	Shader::setMat4("view", MainCamera::getInstance()->GetViewMatrix());
	Shader::setMat4("projection", MainCamera::getInstance()->GetProjectionMatrix());

	Shader::setVec3("objectColor", 1.0f, 1.0f, 1.0f);
	Shader::setVec3("lightColor",  1.0f, 1.0f, 1.0f);
	Shader::setVec3("lightPos", Light::getInstance()->getPosition());
	Shader::setVec3("viewPos", MainCamera::getInstance()->getPosition());

	Shader::setFloat("ambientStrength", _ambientStrength);
	Shader::setFloat("diffuseStrength", _diffuseStrength);
	Shader::setFloat("specularStrength", _specularStrength);
	Shader::setInt("specularPower", (int)pow(2, _specularPower));
}

void GrassShader::renderGUI()
{
	ImGui::SliderFloat("Ambient Strength", &_ambientStrength, 0, 1.0f);
	ImGui::SliderFloat("Diffuse Strength", &_diffuseStrength, 0, 1.0f);
	ImGui::SliderFloat("Specular Strength", &_specularStrength, 0, 1.0f);
	ImGui::SliderInt("Specular Power", &_specularPower, 0, 10);
}

} // namespace CGRA350