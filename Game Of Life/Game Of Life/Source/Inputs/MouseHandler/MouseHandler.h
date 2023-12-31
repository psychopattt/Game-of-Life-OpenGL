#pragma once

struct GLFWwindow;

class MouseHandler
{
	public:
		void HandleMouseScroll(GLFWwindow* window, double offsetX, double offsetY);
		void HandleMouseButton(GLFWwindow* window, int button, int action, int mods);
		void Update();

	private:
		void UpdateZoom();
};
