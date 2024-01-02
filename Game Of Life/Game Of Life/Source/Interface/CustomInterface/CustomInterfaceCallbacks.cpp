#include "CustomInterface.h"

#include <cmath>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "imgui/imgui.h"
#include "Inputs/InputHandler.h"

void CustomInterface::ActivateCallbacks()
{
	glfwSetWindowUserPointer(window, this);
	glfwSetKeyCallback(window, KeyboardCallback);
	glfwSetScrollCallback(window, MouseScrollCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	glfwSetFramebufferSizeCallback(window, ResizeCallback);
}

void CustomInterface::ResizeCallback(GLFWwindow* window, int width, int height)
{
	CustomInterface* gui = (CustomInterface*)glfwGetWindowUserPointer(window);
	gui->height = height;
	gui->width = width;
	gui->ComputeViewportSettings();
}

void CustomInterface::MouseScrollCallback(GLFWwindow* window, double offsetX, double offsetY)
{
	if (!ImGui::GetIO().WantCaptureMouse)
	{
		CustomInterface* gui = (CustomInterface*)glfwGetWindowUserPointer(window);
		gui->inputHandler->HandleMouseScroll(window, offsetX, offsetY);
	}
}

void CustomInterface::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (!ImGui::GetIO().WantCaptureMouse)
	{
		CustomInterface* gui = (CustomInterface*)glfwGetWindowUserPointer(window);
		gui->inputHandler->HandleMouseButton(window, button, action, mods);
	}
}

void CustomInterface::KeyboardCallback(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
	if (!ImGui::GetIO().WantCaptureKeyboard)
	{
		CustomInterface* gui = (CustomInterface*)glfwGetWindowUserPointer(window);
		gui->inputHandler->HandleKeyboard(window, key, scanCode, action, mods);
	}
}
