#include "Interface.h"

#include <cmath>

#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "imgui/imgui.h"
#include "Inputs/InputHandler.h"

void Interface::ActivateCallbacks()
{
	glfwSetWindowUserPointer(window, this);
	glfwSetKeyCallback(window, KeyboardCallback);
	glfwSetScrollCallback(window, MouseScrollCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	glfwSetFramebufferSizeCallback(window, ResizeCallback);
}

void Interface::ResizeCallback(GLFWwindow* window, int width, int height)
{
	Interface* gui = (Interface*)glfwGetWindowUserPointer(window);
	gui->height = height;
	gui->width = width;
	gui->ComputeViewportSettings();
}

void Interface::MouseScrollCallback(GLFWwindow* window, double offsetX, double offsetY)
{
	if (!ImGui::GetIO().WantCaptureMouse)
	{
		Interface* gui = (Interface*)glfwGetWindowUserPointer(window);
		gui->inputHandler->HandleMouseScroll(window, offsetX, offsetY);
	}
}

void Interface::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (!ImGui::GetIO().WantCaptureMouse)
	{
		Interface* gui = (Interface*)glfwGetWindowUserPointer(window);
		gui->inputHandler->HandleMouseButton(window, button, action, mods);
	}
}

void Interface::KeyboardCallback(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
	if (!ImGui::GetIO().WantCaptureKeyboard)
	{
		Interface* gui = (Interface*)glfwGetWindowUserPointer(window);
		gui->inputHandler->HandleKeyboard(window, key, scanCode, action, mods);
	}
}
