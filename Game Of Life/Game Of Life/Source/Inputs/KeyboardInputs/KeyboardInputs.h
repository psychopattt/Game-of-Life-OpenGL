#pragma once

class KeyboardInputs
{
	public:
		void HandleKeyboard(struct GLFWwindow* window, int key, int scanCode, int action, int mods);
		void Update();

	private:
		void ReleaseCapturedKeys();
		void UpdatePan();
};
