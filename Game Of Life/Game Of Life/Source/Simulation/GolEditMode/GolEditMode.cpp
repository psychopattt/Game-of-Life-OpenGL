#include "GolEditMode.h"

#include <cmath>

#include "glad/gl.h"

#include "../GameOfLife.h"
#include "Shaders/Buffers/ComputeBuffer/ComputeBuffer.h"
#include "Simulation/SimulationMath/SimulationMath.h"
#include "Settings/GameOfLifeSettings.h"
#include "Settings/TransformSettings.h"
#include "Settings/MainSettings.h"

GolEditMode::GolEditMode()
{
	gameOfLife = reinterpret_cast<GameOfLife*>(MainSettings::Sim);
}

void GolEditMode::Update()
{
	if (GameOfLifeSettings::EditMode)
	{
		if (!DrawPixels() && !ErasePixels())
		{
			lastPixelX = -1;
			lastPixelY = -1;
		}
	}
}

bool GolEditMode::DrawPixels()
{
	if (TransformSettings::DraggingLeftClick && !TransformSettings::DraggingRightClick)
	{
		ModifyMousePathPixels(true);
		return true;
	}

	return false;
}

bool GolEditMode::ErasePixels()
{
	if (TransformSettings::DraggingRightClick && !TransformSettings::DraggingLeftClick)
	{
		ModifyMousePathPixels(false);
		return true;
	}

	return false;
}

void GolEditMode::ModifyMousePathPixels(bool pixelState)
{
	int currentPixelX, currentPixelY;
	SimulationMath::ComputeMousePixelCoords(currentPixelX, currentPixelY);

	// Modifying a single pixel
	if ((currentPixelX == lastPixelX && currentPixelY == lastPixelY) ||
		lastPixelX == -1 || lastPixelY == -1)
	{
		lastPixelX = currentPixelX;
		lastPixelY = currentPixelY;
		SetPixel(currentPixelX, currentPixelY, pixelState);
		return;
	}

	// Modifying multiple pixels; Interpolate between pixel positions
	int pixelOffsetX = abs(currentPixelX - lastPixelX);
	int pixelOffsetY = abs(currentPixelY - lastPixelY);
	int largestPixelOffset = pixelOffsetX > pixelOffsetY ? pixelOffsetX : pixelOffsetY;

	for (int currentOffset = 1; currentOffset <= largestPixelOffset; currentOffset++)
	{
		double interpolation = static_cast<double>(currentOffset) / largestPixelOffset;

		int interpolatedPixelX = lround(std::lerp(
			lastPixelX, currentPixelX, interpolation
		));

		int interpolatedPixelY = lround(std::lerp(
			lastPixelY, currentPixelY, interpolation
		));

		SetPixel(interpolatedPixelX, interpolatedPixelY, pixelState);
	}
	
	lastPixelX = currentPixelX;
	lastPixelY = currentPixelY;
}

void GolEditMode::SetPixel(int coordX, int coordY, bool state)
{
	int pixelId = SimulationMath::ConvertPixelCoordsToPixelId(coordX, coordY);

	if (pixelId != -1)
	{
		ComputeBuffer* buffer = gameOfLife->GetBuffer(1);
		const int accessType = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT;

		unsigned int* data = reinterpret_cast<unsigned int*>(
			buffer->Map(accessType, pixelId * sizeof(int), sizeof(int))
		);

		if (data)
		{
			data[0] = static_cast<unsigned int>(state);
			buffer->Unmap();
		}
	}
}
