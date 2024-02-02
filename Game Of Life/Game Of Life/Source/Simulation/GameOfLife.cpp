#include "GameOfLife.h"

#include "glad/gl.h"

#include "Shaders/Buffers/Texture/Texture.h"
#include "Shaders/Buffers/DualComputeBuffer/DualComputeBuffer.h"
#include "Simulation/SimulationDrawer/SimulationDrawer.h"
#include "Shaders/ComputeShader/ComputeShader.h"
#include "Settings/GameOfLifeSettings.h"
#include "GolEditMode/GolEditMode.h"

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

	InitializeGameOfLife();
	InitializeDrawing();
	Restart();
}

void GameOfLife::InitializeGameOfLife()
{
	editMode = make_unique<GolEditMode>();
	dualBuffer = make_unique<DualComputeBuffer>(
		sizeof(unsigned int) * width * height
	);

	gameInitShader = make_unique<ComputeShader>("GameOfLifeInit", width, height);
	gameInitShader->SetInt("height", height);
	gameInitShader->SetInt("width", width);
	gameInitShader->SetInt("seed", seed);

	gameShader = make_unique<ComputeShader>("GameOfLife", width, height);
	gameShader->SetInt("height", height);
	gameShader->SetInt("width", width);
}

void GameOfLife::InitializeDrawing()
{
	simDrawer = make_unique<SimulationDrawer>();
	texture = make_unique<Texture>(width, height, GL_RGBA8);

	bufferConverter = make_unique<ComputeShader>("BufferConverter", width, height);
	bufferConverter->SetTextureBinding("dataTexture", texture->GetId());
	bufferConverter->SetInt("height", height);
	bufferConverter->SetInt("width", width);
}

void GameOfLife::ApplySettings()
{
	gameShader->SetBool("edgeLoop", GameOfLifeSettings::EdgeLoop);
	gameShader->SetInt("birthRules", GameOfLifeSettings::BirthRules);
	gameShader->SetInt("survivalRules", GameOfLifeSettings::SurvivalRules);
}

void GameOfLife::Restart()
{
	ApplySettings();
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
	editMode->Update();
	bufferConverter->SetBufferBinding("dataBuffer", dualBuffer->GetId(1));
	bufferConverter->Execute();
	simDrawer->Draw(texture.get());
}

ComputeBuffer* GameOfLife::GetBuffer(int bufferIndex)
{
	return dualBuffer->GetBuffer(bufferIndex);
}

GameOfLife::~GameOfLife() { }
