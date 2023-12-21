#include "InputHandler.h"

#include "./KeyboardInputs/KeyboardInputs.h"
#include "./MouseInputs/MouseInputs.h"
#include "GLFW/glfw3.h"

InputHandler::InputHandler()
{
	mouseInputs = new MouseInputs();
	keyboardInputs = new KeyboardInputs();
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
	mouseInputs->Update();
	keyboardInputs->Update();
}

InputHandler::~InputHandler()
{
	delete keyboardInputs;
	delete mouseInputs;
}
