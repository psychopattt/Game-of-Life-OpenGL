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
		void Restart() override;
		void ApplySettings();
		void Execute() override;
		void Draw() override;
		class ComputeBuffer* GetBuffer(int bufferIndex);
		~GameOfLife();

	private:
		void InitializeTexture();
		void InitializeShaders();

		unique_ptr<class GolEditMode> editMode;

		unique_ptr<class Texture> texture;
		unique_ptr<class SimulationDrawer> simDrawer;
		unique_ptr<class DualComputeBuffer> cellsBuffer;

		unique_ptr<class ComputeShader> initShader;
		unique_ptr<class ComputeShader> updateShader;
		unique_ptr<class ComputeShader> colorShader;
};
