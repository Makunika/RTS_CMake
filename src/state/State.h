#pragma once

#include <GLFW/glfw3.h>
#include "../camera/Camera.h"

struct State
{
	int width;
	int height;

	float lastX;
	float lastY;

	bool firstMouse = true;
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	bool cursorLocked = false;

	Camera* camera;
	GLFWwindow* window;

	glm::mat4 getProjection() {
		return glm::perspective(glm::radians(camera->Zoom), (float)width / (float)height, 0.1f, 100.0f);
	}
};
