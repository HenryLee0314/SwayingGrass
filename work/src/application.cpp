
// std
#include <iostream>
#include <string>
#include <chrono>

// glm
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

// project
#include "application.hpp"
#include "cgra/cgra_geometry.hpp"
#include "cgra/cgra_gui.hpp"
#include "cgra/cgra_image.hpp"
#include "cgra/cgra_shader.hpp"
#include "cgra/cgra_wavefront.hpp"

#include "cgra_log.h"
#include "cgra_heap_calculator.h"
#include "cgra_time_calculator.h"

#include "opencl_manager.h"
#include "opencl_task.h"
#include "fluid_grid.h"
#include "grass_parameters.h"
#include "grass_bundle.h"
#include "floor.h"
#include "light.h"
#include "floor_shadow.h"
#include "main_camera.h"
#include "shader_grass.h"

using namespace std;
using namespace cgra;
using namespace glm;
using namespace CGRA350;

Application::Application(GLFWwindow *window)
	: m_window(window)
	, _fluidShader(CGRA_SRCDIR "/res/shaders/vertexShader/fluid.vs", CGRA_SRCDIR "/res/shaders/fragmentShader/fluid.fs")
{

}


void Application::render() {
	//system("clear");

	// retrieve the window hieght
	int width, height;
	glfwGetFramebufferSize(m_window, &width, &height);

	m_windowsize = vec2(width, height); // update window size
	glViewport(0, 0, width, height); // set the viewport to draw to the entire window

	// clear the back-buffer
	glClearColor(0.3f, 0.3f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// enable flags for normal/forward rendering
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);


	MainCamera::getInstance()->setYaw(m_yaw);
	MainCamera::getInstance()->setPitch(m_pitch);
	MainCamera::getInstance()->setDistance(m_distance);

	mat4 view = MainCamera::getInstance()->GetViewMatrix();
	mat4 proj = MainCamera::getInstance()->GetProjectionMatrix();

	static glm::mat4 model = glm::mat4(1.0f);

	// helpful draw options
	if (m_show_grid) drawGrid(view, proj);
	if (m_show_axis) drawAxis(view, proj);
	glPolygonMode(GL_FRONT_AND_BACK, (m_showWireframe) ? GL_LINE : GL_FILL);
	(m_showWireframe) ? glEnable(GL_PROGRAM_POINT_SIZE) : glDisable(GL_PROGRAM_POINT_SIZE);

	CGRA_ACTIVITY_START(CGRA350);

	FluidGrid::getInstance()->update();

	GrassBundle::getInstance()->update();

	FluidGrid::getInstance()->wait();

	FloorShadow::getInstance()->renderShadow();
	glViewport(0, 0, width, height);

    CGRA_ACTIVITY_START(GRASS_RENDER);
    GrassShader::getInstance()->use();
	GrassBundle::getInstance()->render();
	CGRA_ACTIVITY_END(GRASS_RENDER);	


	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	_fluidShader.use();
	_fluidShader.setMat4("model", model);
	_fluidShader.setMat4("view", view);
	_fluidShader.setMat4("projection", proj);

	_fluidShader.setVec3("objectColor", 1.0f, 1.0f, 1.0f);
	_fluidShader.setVec3("lightColor",  1.0f, 1.0f, 1.0f);
	_fluidShader.setVec3("lightPos", Light::getInstance()->getPosition());
	_fluidShader.setVec3("viewPos", MainCamera::getInstance()->getPosition());
	FluidGrid::getInstance()->render();
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	FloorShadow::getInstance()->render();

	CGRA_ACTIVITY_END(CGRA350);
}


void Application::renderGUI() {
	// setup window
	ImGui::SetNextWindowPos(ImVec2(5, 5), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiSetCond_Once);
	ImGui::Begin("Options", 0);

	// display current camera parameters
	ImGui::Text("Application %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::SliderFloat("Pitch", &m_pitch, -pi<float>() / 2, pi<float>() / 2, "%.2f");
	ImGui::SliderFloat("Yaw", &m_yaw, -pi<float>(), pi<float>(), "%.2f");
	ImGui::SliderFloat("Distance", &m_distance, 0, 100, "%.2f", 2.0f);

	// helpful drawing options
	ImGui::Checkbox("Show axis", &m_show_axis);
	ImGui::SameLine();
	ImGui::Checkbox("Show grid", &m_show_grid);
	ImGui::Checkbox("Wireframe", &m_showWireframe);
	ImGui::SameLine();
	if (ImGui::Button("Screenshot")) rgba_image::screenshot(true);

	ImGui::Separator();

	Light::getInstance()->renderGUI();

	ImGui::Separator();

	GrassParameters::getInstance()->renderGUI();

	ImGui::Separator();

	FluidGrid::getInstance()->renderGUI();

	ImGui::Separator();

	GrassShader::getInstance()->renderGUI();

	ImGui::Separator();

	FloorShadow::getInstance()->renderGUI();

	ImGui::End();
}


void Application::cursorPosCallback(double xpos, double ypos) {
	if (m_leftMouseDown) {
		vec2 whsize = m_windowsize / 2.0f;

		// clamp the pitch to [-pi/2, pi/2]
		m_pitch += float(acos(glm::clamp((m_mousePosition.y - whsize.y) / whsize.y, -1.0f, 1.0f))
		                 - acos(glm::clamp((float(ypos) - whsize.y) / whsize.y, -1.0f, 1.0f)));
		m_pitch = float(glm::clamp(m_pitch, -pi<float>() / 2, pi<float>() / 2));

		// wrap the yaw to [-pi, pi]
		m_yaw += float(acos(glm::clamp((m_mousePosition.x - whsize.x) / whsize.x, -1.0f, 1.0f))
		               - acos(glm::clamp((float(xpos) - whsize.x) / whsize.x, -1.0f, 1.0f)));
		if (m_yaw > pi<float>()) m_yaw -= float(2 * pi<float>());
		else if (m_yaw < -pi<float>()) m_yaw += float(2 * pi<float>());
	}

	// updated mouse position
	m_mousePosition = vec2(xpos, ypos);
}


void Application::mouseButtonCallback(int button, int action, int mods) {
	(void)mods; // currently un-used

	// capture is left-mouse down
	if (button == GLFW_MOUSE_BUTTON_LEFT)
		m_leftMouseDown = (action == GLFW_PRESS); // only other option is GLFW_RELEASE
}


void Application::scrollCallback(double xoffset, double yoffset) {
	(void)xoffset; // currently un-used
	m_distance *= pow(1.1f, -yoffset);
}


void Application::keyCallback(int key, int scancode, int action, int mods) {
	(void)key, (void)scancode, (void)action, (void)mods; // currently un-used
}


void Application::charCallback(unsigned int c) {
	(void)c; // currently un-used
}
