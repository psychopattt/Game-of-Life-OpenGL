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

		const int accessType = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT;
		unsigned int* data = reinterpret_cast<unsigned int*>(
			buffer->Map(accessType, pixelId * sizeof(int), sizeof(int))
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

	if (pixelX < 0 || pixelX >= simWidth || pixelY < 0 || pixelY >= simHeight)
		return -1;

	return pixelY * simWidth + pixelX;
}
