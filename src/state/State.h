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
};
