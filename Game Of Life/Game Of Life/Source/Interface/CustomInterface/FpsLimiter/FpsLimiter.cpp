#include "FpsLimiter.h"
#include "GLFW/glfw3.h"

FpsLimiter::FpsLimiter() : FpsLimiter(0) { }

FpsLimiter::FpsLimiter(double targetFps)
{
	SetTargetFps(targetFps);
	lastUpdateTime = glfwGetTime() - targetFrametime;
}

void FpsLimiter::SetTargetFps(double targetFps)
{
	disabled = targetFps > 9999;

	if (!disabled)
	{
		if (targetFps < 0.01)
			targetFps = 0.01;

		targetFrametime = 1 / targetFps;
	}
}

bool FpsLimiter::Update()
{
	if (disabled)
		return true;

	double currentTime = glfwGetTime();
	double timeDiff = currentTime - lastUpdateTime;

	if (timeDiff >= targetFrametime)
	{
		lastUpdateTime = currentTime;
		return true;
	}

	return false;
}
