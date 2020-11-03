#include "floor_shadow.h"
#include "floor.h"
#include "grass_bundle.h"

#include "shader_first_pass.h"
#include "shader_second_pass.h"

#include "opengl.hpp"

#include "cgra_log.h"
#include "cgra_heap_calculator.h"
#include "cgra_time_calculator.h"

#include "cgra/cgra_gui.hpp"

#include "light.h"

namespace CGRA350 {

#ifdef __APPLE__
const GLuint SHADOW_WIDTH = 2000 * 2, SHADOW_HEIGHT = 2000 * 2;
#else
const GLuint SHADOW_WIDTH = 2000, SHADOW_HEIGHT = 2000;
#endif

FloorShadow* FloorShadow::_instance = nullptr;

FloorShadow* FloorShadow::getInstance()
{
	if (_instance == nullptr) {
		_instance = new FloorShadow();
	}

	return _instance;
}

FloorShadow::FloorShadow()
	: _depthMapFBO(0)
	, _depthMap(0)
	, _renderShadow(true)
	, _shadowDebug(false)
{

	glGenFramebuffers(1, &_depthMapFBO);

	glGenTextures(1, &_depthMap);
	glBindTexture(GL_TEXTURE_2D, _depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, _depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FloorShadow::~FloorShadow()
{

}

void FloorShadow::shadowDebug()
{
	static Shader shadowDebugShader(CGRA_SRCDIR "/res/shaders/vertexShader/shadow_debug.vs", CGRA_SRCDIR "/res/shaders/fragmentShader/shadow_debug.fs");
	static unsigned int quadVAO = 0;
	static unsigned int quadVBO;
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}

	if (_shadowDebug) {
		shadowDebugShader.use();
		shadowDebugShader.setFloat("near_plane", Light::getInstance()->getNearPlane());
		shadowDebugShader.setFloat("far_plane", Light::getInstance()->getFarPlane());
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _depthMap);

		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}
}

void FloorShadow::renderGUI()
{
	ImGui::Checkbox("Render Shadow", &_renderShadow);
	ImGui::Checkbox("Debug Shadow", &_shadowDebug);
}

void FloorShadow::renderShadow()
{
	CGRA_ACTIVITY_START(TWO_PASS);

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, _depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	if (_renderShadow) {
		FirstPassShader::getInstance()->useGrassShader();
		GrassBundle::getInstance()->render();
		// FirstPassShader::getInstance()->useFloorShader();
		// Floor::getInstance()->render();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	CGRA_ACTIVITY_END(TWO_PASS);
}

void FloorShadow::render()
{

	SecondPassShader::getInstance()->useFloorShader();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _depthMap);
	Floor::getInstance()->render();


	shadowDebug();
}

} // namespace CGRA350