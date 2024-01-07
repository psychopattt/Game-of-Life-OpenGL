#include "Simulation.h"

#include "Simulation/SimulationDrawer/SimulationDrawer.h"
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

void Simulation::Draw()
{
	simDrawer->Draw();
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

Simulation::~Simulation() { }
