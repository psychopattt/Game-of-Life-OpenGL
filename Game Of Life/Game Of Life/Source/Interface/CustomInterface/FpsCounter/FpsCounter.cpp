#include "FpsCounter.h"
#include "GLFW/glfw3.h"

FpsCounter::FpsCounter(double updatesPerSecond)
{
	updateInterval = 1 / updatesPerSecond;
}

bool FpsCounter::Update()
{
	frameCounter++;
	double currentTime = glfwGetTime();
	double timeDiff = currentTime - lastUpdateTime;

	if (timeDiff >= updateInterval)
	{
		ms = 1000 * timeDiff / frameCounter;
		fps = frameCounter / timeDiff;
		lastUpdateTime = currentTime;
		frameCounter = 0;

		return true;
	}

	return false;
}

std::string FpsCounter::ToString()
{
	char buffer[25];

	snprintf(
		buffer, sizeof(buffer) / sizeof(*buffer),
		"%.2f fps / %.3f ms", fps, ms
	);

	return buffer;
}
