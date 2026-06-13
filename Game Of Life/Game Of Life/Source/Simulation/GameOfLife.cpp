#include "GameOfLife.h"

#include "glad/gl.h"

#include "Shaders/Buffers/Texture/Texture.h"
#include "Shaders/Buffers/DualComputeBuffer/DualComputeBuffer.h"
#include "Simulation/SimulationDrawer/SimulationDrawer.h"
#include "Shaders/ComputeShader/ComputeShader.h"
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

	simDrawer = make_unique<SimulationDrawer>();
	cellsBuffer = make_unique<DualComputeBuffer>(
		sizeof(unsigned int) * width * height
	);

	InitializeTexture();
	InitializeShaders();
	Restart();
}

void GameOfLife::InitializeTexture()
{
	texture = make_unique<Texture>(width, height, GL_R8);

	int swizzle[] = { GL_RED, GL_RED, GL_RED, GL_ONE };
	glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle);
}

void GameOfLife::InitializeShaders()
{
	initShader = make_unique<ComputeShader>("Initialize", width, height);
	initShader->SetUniform("size", width, height);
	initShader->SetUniform("seed", seed);

	updateShader = make_unique<ComputeShader>("Update", width, height);
	updateShader->SetUniform("size", width, height);

	colorShader = make_unique<ComputeShader>("Color", width, height);
	colorShader->SetTextureBinding("texture", texture->GetId());
	colorShader->SetUniform("size", width, height);

	drawShader = make_unique<ComputeShader>("Draw", width, height);
	drawShader->SetUniform("size", width, height);
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
	if (GolSettings::Editing)
	{
		drawShader->SetBufferBinding("cellsBuffer", cellsBuffer->GetId(1));
		drawShader->SetUniform("lastPosition", GolSettings::LastPositionX, GolSettings::LastPositionY);
		drawShader->SetUniform("currentPosition", GolSettings::CurrentPositionX, GolSettings::CurrentPositionY);
		drawShader->SetUniform("drawing", GolSettings::Drawing);
		drawShader->Execute();
	}

	colorShader->SetBufferBinding("cellsBuffer", cellsBuffer->GetId(1));
	colorShader->Execute();

	simDrawer->Draw(texture.get());
}

GameOfLife::~GameOfLife() { }
