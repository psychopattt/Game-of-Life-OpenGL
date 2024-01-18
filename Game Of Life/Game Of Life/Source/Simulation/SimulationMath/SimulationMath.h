#pragma once

namespace SimulationMath
{
	double ScaleZoom(double zoom);
	double GetMaxPanAtZoom(double zoom);

	double GetVisibleWorldSize(double worldSize, double screenSize, double viewportSize);
	int ConvertWorldCoordToPixelCoord(double worldCoord, double textureSize);

	void ComputeScreenCenterWorldCoords(double& screenCenterWorldX, double& screenCenterWorldY);

	double GetMouseWorldCoordOffsetCenter(double screenCoord, double screenSize,
		double viewportSize, double viewportScale, double worldSize);
	void ComputeMouseWorldCoords(double& worldX, double& worldY);
};
