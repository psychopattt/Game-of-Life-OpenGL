#pragma once

#include <memory>

#include "Simulation/Simulation.h"

using std::unique_ptr;

class GameOfLife : public Simulation
{
	public:
		GameOfLife(int width, int height, unsigned int seed = 0);
		void Initialize() override;
		void Initialize(int width, int height, unsigned int seed = 0) override;
		void InitializeGameOfLife();
		void InitializeDrawing();
		void ApplySettings();
		void Restart() override;
		void Execute() override;
		void Draw() override;
		class ComputeBuffer* GetBuffer(int bufferIndex);
		~GameOfLife();

	private:
		unique_ptr<class GolEditMode> editMode;
		unique_ptr<class DualComputeBuffer> dualBuffer;
		unique_ptr<class ComputeShader> gameInitShader;
		unique_ptr<class ComputeShader> gameShader;

		unique_ptr<class Texture> texture;
		unique_ptr<class ComputeShader> bufferConverter;
		unique_ptr<class SimulationDrawer> simDrawer;
};
