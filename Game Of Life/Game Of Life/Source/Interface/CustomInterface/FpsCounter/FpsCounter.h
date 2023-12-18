#pragma once

#include <string>

class FpsCounter
{
	public:
		FpsCounter(double updatesPerSecond);
		bool Update();
		std::string ToString();

	private:
		double ms = 0;
		double fps = 0;
		double lastUpdateTime = 0;
		unsigned int frameCounter = 0;
		double updateInterval;
};
