#pragma once

class KeyboardInputs
{
	public:
		void HandleKeyboard(struct GLFWwindow* window, int key, int scanCode, int action, int mods);
		void Update(double deltaTime);

	private:
		void ReleaseCapturedKeys();
		void UpdatePan(double deltaTime);
};
