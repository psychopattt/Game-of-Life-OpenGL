#pragma once

#include <memory>

#include "Simulation/Simulation.h"

using std::unique_ptr;

class GameOfLife : public Simulation
{
	public:
		GameOfLife(int width, int height, unsigned int seed = 0);
		void Initialize(int width, int height, unsigned int seed = 0);
		void Restart();
		void Execute();
		void Draw();
		~GameOfLife();

	private:
		unique_ptr<class SimulationDrawer> simDrawer;
		unique_ptr<class DualComputeBuffer> dualBuffer;
		unique_ptr<class ComputeShader> gameInitShader;
		unique_ptr<class ComputeShader> gameShader;
};
