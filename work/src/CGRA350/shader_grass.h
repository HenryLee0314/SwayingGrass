#ifndef SHADER_GRASS_H
#define SHADER_GRASS_H

#include "shader.h"

namespace CGRA350 {

class GrassShader : public Shader
{
public:
	static GrassShader* getInstance();

	virtual void use();

	virtual void renderGUI();

private:
	GrassShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr, const char* tessellationControlPath = nullptr, const char* tessellationEvaluationPath = nullptr);
	virtual ~GrassShader();
	GrassShader(const GrassShader&);
	GrassShader& operator = (const GrassShader&);

private:
	static GrassShader* _instance;

	float _ambientStrength;
	float _diffuseStrength;
	float _specularStrength;
	int _specularPower;
};

} // namespace CGRA350

#endif // SHADER_GRASS_H