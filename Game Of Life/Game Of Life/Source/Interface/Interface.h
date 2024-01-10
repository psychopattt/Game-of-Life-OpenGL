#pragma once

#include <memory>
#include <string>

using std::unique_ptr, std::string;

class Simulation;
class FpsLimiter;
class FpsCounter;
struct GLFWwindow;
enum UpdateType : char;

class Interface
{
	public:
		Interface(int width, int height, string title);
		bool ShouldExit() const;
		UpdateType Update();
		void TriggerResize() const;
		void ApplyFullscreenState() const;
		void SetTargetFps(float targetFps) const;
		void StepFrame();
		int GetWidth() const;
		int GetHeight() const;
		void GetViewportSize(int& width, int& height) const;
		const double* GetMetrics() const;
		class WindowTitle* GetTitle() const;
		void GetMousePosition(double* posX, double* posY) const;
		GLFWwindow* GetWindow() const;
		~Interface();

	private:
		int width;
		int height;
		const int initialWidth;
		const int initialHeight;
		bool stepFrame = false;
		double metrics[4] = { };

		GLFWwindow* window;
		unique_ptr<class WindowTitle> title;
		unique_ptr<FpsLimiter> uiFpsLimiter;
		unique_ptr<FpsCounter> uiFpsCounter;
		unique_ptr<FpsLimiter> gameFpsLimiter;
		unique_ptr<FpsCounter> gameFpsCounter;
		unique_ptr<class ImGuiHandler> imGuiHandler;
		unique_ptr<class InputHandler> inputHandler;

		void SetVersion(char major, char minor) const;
		void CreateWindow();
		void InitializeGlad();
		void CreateFpsHandlers();
		void ActivateCallbacks();
		void UpdateTitle() const;
		void ComputeViewportSettings();
		void ComputeViewportSize(int& viewportSize, int& sizeOffset, double& viewportScale,
			int simSize1, int simSize2, int windowSize1, int windowSize2, int maxViewportSize);

		static void ResizeCallback(GLFWwindow* window, int width, int height);
		static void MouseScrollCallback(GLFWwindow* window, double offsetX, double offsetY);
		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		static void KeyboardCallback(GLFWwindow* window, int key, int scanCode, int action, int mods);
};
