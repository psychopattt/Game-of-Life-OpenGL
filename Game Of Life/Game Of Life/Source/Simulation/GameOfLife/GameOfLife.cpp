#include "GameOfLife.h"

#include "Shaders/Buffers/DualComputeBuffer/DualComputeBuffer.h"
#include "Simulation/SimulationDrawer/SimulationDrawer.h"
#include "Shaders/ComputeShader/ComputeShader.h"

using std::make_unique;

GameOfLife::GameOfLife(int width, int height, unsigned int seed) :
	Simulation(width, height, seed) { };

void GameOfLife::Initialize(int width, int height, unsigned int seed)
{
	this->width = width;
	this->height = height;
	this->seed = seed;

	simDrawer = make_unique<SimulationDrawer>(width, height);
	InitializeDualBuffer();

	gameInitShader = make_unique<ComputeShader>("GameOfLifeInit", width, height);
	gameInitShader->SetInt("width", width);

	gameShader = make_unique<ComputeShader>("GameOfLife", width, height);
	gameShader->SetInt("height", height);
	gameShader->SetInt("width", width);

	Restart();
}

void GameOfLife::InitializeDualBuffer()
{
	size_t bufferSize = static_cast<size_t>(width) * height;
	unsigned int* bufferData = new unsigned int[bufferSize];

	dualBuffer = make_unique<DualComputeBuffer>(
		bufferData, bufferSize * sizeof(*bufferData)
	);

	delete[] bufferData;
}

void GameOfLife::Restart()
{
	gameInitShader->Execute();
}

void GameOfLife::Execute()
{
	gameShader->Execute();
	dualBuffer->Swap();
}

GameOfLife::~GameOfLife() { }
