#include "GameOfLife.h"

#include "Shaders/Buffers/DualComputeBuffer/DualComputeBuffer.h"
#include "Simulation/SimulationDrawer/SimulationDrawer.h"
#include "Shaders/ComputeShader/ComputeShader.h"

using std::make_unique;

GameOfLife::GameOfLife(int width, int height, unsigned int seed) :
	Simulation(width, height, seed) { };

void GameOfLife::Initialize()
{
	Initialize(width, height, seed);
}

void GameOfLife::Initialize(int width, int height, unsigned int seed)
{
	Simulation::Initialize(width, height, seed);

	simDrawer = make_unique<SimulationDrawer>(width, height);
	dualBuffer = make_unique<DualComputeBuffer>(
		sizeof(unsigned int) * width * height
	);

	gameInitShader = make_unique<ComputeShader>("GameOfLifeInit", width, height);
	gameInitShader->SetInt("width", width);
	gameInitShader->SetInt("seed", seed);

	gameShader = make_unique<ComputeShader>("GameOfLife", width, height);
	gameShader->SetInt("height", height);
	gameShader->SetInt("width", width);

	Restart();
}

void GameOfLife::Restart()
{
	gameInitShader->SetBufferBinding("dataBuffer", dualBuffer->GetId(1));
	gameInitShader->Execute();
}

void GameOfLife::Execute()
{
	dualBuffer->Swap();
	gameShader->SetBufferBinding("inputBuffer", dualBuffer->GetId(0));
	gameShader->SetBufferBinding("outputBuffer", dualBuffer->GetId(1));
	gameShader->Execute();
}

void GameOfLife::Draw()
{
	simDrawer->Draw(dualBuffer->GetId(1));
}

GameOfLife::~GameOfLife() { }
