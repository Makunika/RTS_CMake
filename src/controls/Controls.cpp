#include "Controls.h"

State* currentState;

Controls::Controls(State* state)
{
	currentState = state;
}

void toggleCursor(GLFWwindow* window)
{
	currentState->cursorLocked = !currentState->cursorLocked;
	glfwSetInputMode(window, GLFW_CURSOR, currentState->cursorLocked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (currentState->firstMouse)
    {
        currentState->lastX = xpos;
        currentState->lastY = ypos;
        currentState->firstMouse = false;
    }

    float xoffset = xpos - currentState->lastX;
    float yoffset = currentState->lastY - ypos; // перевернуто, так как y-координаты идут снизу вверх

    currentState->lastX = xpos;
    currentState->lastY = ypos;

    currentState->camera->processMouseMovement(xoffset, yoffset);
}

void resizeCallback(GLFWwindow* window, int width, int height)
{
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    currentState->camera->processMouseScroll(yoffset);
}





