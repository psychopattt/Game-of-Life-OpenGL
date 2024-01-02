#pragma once

#include <string>

using std::string;

class FpsLimiter;
class FpsCounter;
struct GLFWwindow;
enum UpdateType : char;

class CustomInterface
{
	public:
		CustomInterface(int width, int height, int simWidth, int simHeight, string title);
		bool ShouldExit() const;
		UpdateType Update();
		void ApplyFullscreenState() const;
		void SetTargetFps(float targetFps) const;
		void StepFrame();
		int GetWidth() const;
		int GetHeight() const;
		int GetSimWidth() const;
		int GetSimHeight() const;
		void GetViewportSize(int& width, int& height) const;
		const double* GetMetrics() const;
		class WindowTitle* GetTitle() const;
		void GetMousePosition(double* posX, double* posY) const;
		GLFWwindow* GetWindow() const;
		~CustomInterface();

	private:
		int width;
		int height;
		int simWidth;
		int simHeight;
		const int initialWidth;
		const int initialHeight;
		bool stepFrame = false;
		double metrics[4] = { };

		GLFWwindow* window;
		class WindowTitle* title;
		FpsLimiter* uiFpsLimiter;
		FpsCounter* uiFpsCounter;
		FpsLimiter* gameFpsLimiter;
		FpsCounter* gameFpsCounter;
		class ImGuiWindow* imGuiWindows;
		class ImGuiHandler* imGuiHandler;
		class InputHandler* inputHandler;

		void SetVersion(char major, char minor) const;
		void CreateWindow();
		void InitializeGlad();
		void CreateFpsHandlers();
		void ActivateCallbacks();
		void UpdateTitle() const;
		void ComputeViewportSettings();

		static void ResizeCallback(GLFWwindow* window, int width, int height);
		static void MouseScrollCallback(GLFWwindow* window, double offsetX, double offsetY);
		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		static void KeyboardCallback(GLFWwindow* window, int key, int scanCode, int action, int mods);
};
