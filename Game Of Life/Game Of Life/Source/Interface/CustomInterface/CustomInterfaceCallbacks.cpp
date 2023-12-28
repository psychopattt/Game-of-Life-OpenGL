#include "CustomInterface.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "imgui/imgui.h"
#include "Inputs/InputHandler.h"

void CustomInterface::ActivateCallbacks()
{
	glfwSetWindowUserPointer(window, &inputHandler);
	glfwSetKeyCallback(window, KeyboardCallback);
	glfwSetScrollCallback(window, MouseScrollCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	glfwSetFramebufferSizeCallback(window, ResizeCallback);
}

void CustomInterface::ResizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void CustomInterface::MouseScrollCallback(GLFWwindow* window, double offsetX, double offsetY)
{
	if (!ImGui::GetIO().WantCaptureMouse)
	{
		InputHandler* inputHandler = (InputHandler*)glfwGetWindowUserPointer(window);
		inputHandler->HandleMouseScroll(window, offsetX, offsetY);
	}
}

void CustomInterface::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (!ImGui::GetIO().WantCaptureMouse)
	{
		InputHandler* inputHandler = (InputHandler*)glfwGetWindowUserPointer(window);
		inputHandler->HandleMouseButton(window, button, action, mods);
	}
}

void CustomInterface::KeyboardCallback(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
	if (!ImGui::GetIO().WantCaptureKeyboard)
	{
		InputHandler* inputHandler = (InputHandler*)glfwGetWindowUserPointer(window);
		inputHandler->HandleKeyboard(window, key, scanCode, action, mods);
	}
}
