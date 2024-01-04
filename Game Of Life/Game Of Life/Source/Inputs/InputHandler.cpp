#include "InputHandler.h"

#include "./KeyboardInputs/KeyboardInputs.h"
#include "./MouseInputs/MouseInputs.h"
#include "GLFW/glfw3.h"

using std::make_unique;

InputHandler::InputHandler()
{
	mouseInputs = make_unique<MouseInputs>();
	keyboardInputs = make_unique<KeyboardInputs>();
}

void InputHandler::HandleMouseScroll(GLFWwindow* window, double offsetX, double offsetY)
{
	mouseInputs->HandleMouseScroll(window, offsetX, offsetY);
}

void InputHandler::HandleMouseButton(GLFWwindow* window, int button, int action, int mods)
{
	mouseInputs->HandleMouseButton(window, button, action, mods);
}

void InputHandler::HandleKeyboard(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
	keyboardInputs->HandleKeyboard(window, key, scanCode, action, mods);
}

void InputHandler::Update()
{
	double currentTime = glfwGetTime();
	double deltaTime = currentTime - lastTime;
	lastTime = currentTime;

	mouseInputs->Update();
	keyboardInputs->Update(deltaTime);
}

InputHandler::~InputHandler() { }
