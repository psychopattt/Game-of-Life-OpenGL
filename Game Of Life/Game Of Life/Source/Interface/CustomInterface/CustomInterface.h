#pragma once

#include <string>

using namespace std;

class FpsLimiter;
class FpsCounter;
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
		struct GLFWwindow* GetWindow() const;
		void Destroy() const;

	private:
		int width = 0;
		int height = 0;
		double metrics[4] = { };

		struct GLFWwindow* window;
		class WindowTitle* title;
		FpsLimiter* uiFpsLimiter;
		FpsCounter* uiFpsCounter;
		FpsLimiter* gameFpsLimiter;
		FpsCounter* gameFpsCounter;
		class ImGuiWindow* imGuiWindows;

		void SetVersion(char major, char minor) const;
		void CreateWindow();
		void InitializeGlad();
		void CreateFpsHandlers();
		void UpdateTitle() const;

		static void ResizeCallback(struct GLFWwindow* window, int width, int height);
};
