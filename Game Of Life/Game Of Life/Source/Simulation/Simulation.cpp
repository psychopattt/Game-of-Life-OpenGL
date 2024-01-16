#include "Simulation.h"

#include "Settings/Settings.h"

Simulation::Simulation(int width, int height, unsigned int seed) :
	width(width), height(height), seed(seed)
{
	Settings::Sim = this;
}

void Simulation::Initialize()
{
	Initialize(width, height, seed);
}

void Simulation::Restart(int width, int height, unsigned int seed)
{
	if (this->width != width || this->height != height || this->seed != seed)
		Initialize(width, height, seed);
	else
		Restart();
}

int Simulation::GetWidth() const
{
	return width;
}

int Simulation::GetHeight() const
{
	return height;
}

unsigned int Simulation::GetSeed() const
{
	return seed;
}
