#pragma once
#include <GLFW/glfw3.h>
#include "../state/State.h"


class Controls
{
public:
	Controls(State* state);
};

void toggleCursor(GLFWwindow* window);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void resizeCallback(GLFWwindow* window, int width, int height);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);


