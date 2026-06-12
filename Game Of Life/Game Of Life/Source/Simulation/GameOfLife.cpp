#include "GameOfLife.h"

#include "glad/gl.h"

#include "Shaders/Buffers/Texture/Texture.h"
#include "Shaders/Buffers/DualComputeBuffer/DualComputeBuffer.h"
#include "Simulation/SimulationDrawer/SimulationDrawer.h"
#include "Shaders/ComputeShader/ComputeShader.h"
#include "GolEditMode/GolEditMode.h"
#include "Settings/GolSettings.h"

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

	texture = make_unique<Texture>(width, height, GL_RGBA8);
	editMode = make_unique<GolEditMode>();
	simDrawer = make_unique<SimulationDrawer>();
	cellsBuffer = make_unique<DualComputeBuffer>(
		sizeof(unsigned int) * width * height
	);

	InitializeShaders();
	Restart();
}

void GameOfLife::InitializeShaders()
{
	initShader = make_unique<ComputeShader>("Initialize", width, height);
	initShader->SetUniform("height", height);
	initShader->SetUniform("width", width);
	initShader->SetUniform("seed", seed);

	updateShader = make_unique<ComputeShader>("Update", width, height);
	updateShader->SetUniform("height", height);
	updateShader->SetUniform("width", width);

	colorShader = make_unique<ComputeShader>("Color", width, height);
	colorShader->SetTextureBinding("texture", texture->GetId());
	colorShader->SetUniform("height", height);
	colorShader->SetUniform("width", width);
}

void GameOfLife::Restart()
{
	ApplySettings();
	initShader->SetBufferBinding("cellsBuffer", cellsBuffer->GetId(1));
	initShader->Execute();
}

void GameOfLife::ApplySettings()
{
	updateShader->SetUniform("edgeLoop", GolSettings::EdgeLoop);
	updateShader->SetUniform("birthRules", GolSettings::BirthRules);
	updateShader->SetUniform("survivalRules", GolSettings::SurvivalRules);
}

void GameOfLife::Execute()
{
	cellsBuffer->Swap();
	updateShader->SetBufferBinding("inputBuffer", cellsBuffer->GetId(0));
	updateShader->SetBufferBinding("outputBuffer", cellsBuffer->GetId(1));
	updateShader->Execute();
}

void GameOfLife::Draw()
{
	editMode->Update();
	colorShader->SetBufferBinding("cellsBuffer", cellsBuffer->GetId(1));
	colorShader->Execute();
	simDrawer->Draw(texture.get());
}

ComputeBuffer* GameOfLife::GetBuffer(int bufferIndex)
{
	return cellsBuffer->GetBuffer(bufferIndex);
}

GameOfLife::~GameOfLife() { }
