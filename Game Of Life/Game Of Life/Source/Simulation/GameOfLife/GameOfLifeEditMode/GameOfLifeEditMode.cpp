#include "GameOfLifeEditMode.h"

#include "glad/gl.h"

#include "../GameOfLife.h"
#include "Shaders/Buffers/ComputeBuffer/ComputeBuffer.h"
#include "Simulation/SimulationMath/SimulationMath.h"
#include "Settings/TransformSettings.h"
#include "Settings/Settings.h"

void GameOfLifeEditMode::Update()
{
	if (Settings::EditMode)
	{
		DrawPixels();
		ErasePixels();
	}
}

void GameOfLifeEditMode::DrawPixels()
{
	if (TransformSettings::DraggingLeftClick && !TransformSettings::DraggingRightClick)
		EditPixels(true);
}

void GameOfLifeEditMode::ErasePixels()
{
	if (TransformSettings::DraggingRightClick && !TransformSettings::DraggingLeftClick)
		EditPixels(false);
}

void GameOfLifeEditMode::EditPixels(bool pixelState)
{
	int pixelId = ComputeMousePixelIndex();

	if (pixelId != -1)
	{
		GameOfLife* gameOfLife = reinterpret_cast<GameOfLife*>(Settings::Sim);
		ComputeBuffer* buffer = gameOfLife->GetBuffer(1);

		unsigned int* data = reinterpret_cast<unsigned int*>(
			buffer->Map(GL_MAP_WRITE_BIT, pixelId * sizeof(int), sizeof(int))
		);
		
		if (data)
		{
			data[0] = static_cast<unsigned int>(pixelState);
			buffer->Unmap();
		}
	}
}

int GameOfLifeEditMode::ComputeMousePixelIndex()
{
	double worldX, worldY;
	SimulationMath::ComputeMouseWorldCoords(worldX, worldY);

	int simWidth = Settings::Sim->GetWidth();
	int simHeight = Settings::Sim->GetHeight();

	int pixelX = SimulationMath::ConvertWorldCoordToPixelCoord(worldX, simWidth);
	int pixelY = SimulationMath::ConvertWorldCoordToPixelCoord(worldY, simHeight);

	int pixelId = pixelY * simWidth + pixelX;

	if (pixelId < 0 || pixelId >= simWidth * simHeight)
		return -1;

	return pixelId;
}
