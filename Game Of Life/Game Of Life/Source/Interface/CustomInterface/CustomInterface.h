#pragma once

#include <string>

using namespace std;

class FpsLimiter;
class FpsCounter;
struct GLFWwindow;
enum UpdateType : char;

class CustomInterface
{
	public:
		CustomInterface(int width, int height, string title);
		bool ShouldExit() const;
		UpdateType Update();
		void ApplyFullscreenState() const;
		void SetTargetFps(double targetFps) const;
		int GetWidth() const;
		int GetHeight() const;
		const double* GetMetrics() const;
		class WindowTitle* GetTitle();
		GLFWwindow* GetWindow() const;
		void Destroy() const;

	private:
		int width = 0;
		int height = 0;
		double metrics[4] = { };

		GLFWwindow* window;
		class WindowTitle* title;
		FpsLimiter* uiFpsLimiter;
		FpsCounter* uiFpsCounter;
		FpsLimiter* gameFpsLimiter;
		FpsCounter* gameFpsCounter;
		class ImGuiWindow* imGuiWindows;
		class InputHandler* inputHandler;

		void SetVersion(char major, char minor) const;
		void CreateWindow();
		void InitializeGlad();
		void CreateFpsHandlers();
		void ActivateCallbacks();
		void UpdateTitle() const;

		static void ResizeCallback(GLFWwindow* window, int width, int height);
		static void MouseScrollCallback(GLFWwindow* window, double offsetX, double offsetY);
		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		static void KeyboardCallback(GLFWwindow* window, int key, int scanCode, int action, int mods);
};
