#pragma once

struct GLFWwindow;

class InputHandler
{
	public:
		InputHandler();
		void HandleMouseScroll(GLFWwindow* window, double offsetX, double offsetY);
		void HandleMouseButton(GLFWwindow* window, int button, int action, int mods);
		void HandleKeyboard(GLFWwindow* window, int key, int scanCode, int action, int mods);
		void Update();
		~InputHandler();

	private:
		class MouseInputs* mouseInputs;
		class KeyboardInputs* keyboardInputs;
};
