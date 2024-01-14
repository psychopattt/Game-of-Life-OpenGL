#pragma once

class KeyboardHandler
{
	public:
		void HandleKeyboard(struct GLFWwindow* window,
			int key, int scanCode, int action, int mods);
		void Update(double deltaTime);

	private:
		void ApplyFrameStep(int key, int action);
		void ApplySpeedMultiplier();
		void ApplyPan(double deltaTime);
};
