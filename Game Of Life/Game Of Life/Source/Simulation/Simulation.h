#pragma once

#include <memory>

class Simulation
{
	public:
		virtual void Initialize();
		virtual void Initialize(int width, int height, unsigned int seed = 0) = 0;
		virtual void Restart() = 0;
		virtual void Execute() = 0;
		virtual void Draw();
		~Simulation();

		int GetWidth() const;
		int GetHeight() const;
		unsigned int GetSeed() const;

	protected:
		Simulation(int width, int height, unsigned int seed = 0);

		int width;
		int height;
		unsigned int seed;
		std::unique_ptr<class SimulationDrawer> simDrawer;
};
