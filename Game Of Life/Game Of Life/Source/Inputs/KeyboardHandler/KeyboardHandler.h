#pragma once

class KeyboardHandler
{
	public:
		void HandleKeyboard(struct GLFWwindow* window, int key, int scanCode, int action, int mods);
		void Update(double deltaTime);

	private:
		void ReleaseCapturedKeys();
		void UpdateSpeedMultiplier();
		void UpdatePan(double deltaTime);
};
