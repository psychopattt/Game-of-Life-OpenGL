#pragma once

class ImGuiHandler
{
	public:
		static void Initialize(struct GLFWwindow* glfwWindow);
		static void Render();
		static void Destroy();

	private:
		static void LoadFonts();
		static void ApplyStyle();
		static void BeginRender();
		static void EndRender();
};
