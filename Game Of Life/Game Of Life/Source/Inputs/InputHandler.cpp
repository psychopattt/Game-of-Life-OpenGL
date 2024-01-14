#include "InputHandler.h"

#include "GLFW/glfw3.h"

#include "MouseHandler/MouseHandler.h"
#include "KeyboardHandler/KeyboardHandler.h"

using std::make_unique;

InputHandler::InputHandler()
{
	mouseHandler = make_unique<MouseHandler>();
	keyboardHandler = make_unique<KeyboardHandler>();
}

void InputHandler::HandleMouseScroll(GLFWwindow* window, double offsetX, double offsetY)
{
	mouseHandler->HandleMouseScroll(window, offsetX, offsetY);
}

void InputHandler::HandleMouseButton(GLFWwindow* window, int button, int action, int mods)
{
	mouseHandler->HandleMouseButton(window, button, action, mods);
}

void InputHandler::HandleKeyboard(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
	keyboardHandler->HandleKeyboard(window, key, scanCode, action, mods);
}

void InputHandler::Update()
{
	double deltaTime = ComputeDeltaTime();
	keyboardHandler->Update(deltaTime);
	mouseHandler->Update(deltaTime);
}

double InputHandler::ComputeDeltaTime()
{
	double currentTime = glfwGetTime();
	double deltaTime = currentTime - lastTime;
	lastTime = currentTime;

	return deltaTime;
}

InputHandler::~InputHandler() { }
