#pragma once

#include <memory>

class SimulationTransforms
{
	public:
		SimulationTransforms(double* quadVertices, size_t quadVertexCount);
		bool ApplyTransforms();

	private:
		double* const quadVertices;
		const size_t quadVertexCount;
		std::unique_ptr<const double[]> initialQuadVertices;

		long long lastPanX = 0;
		long long lastPanY = 0;
		unsigned short lastZoom = 0;

		bool ApplyPanOffset();
		long long ComputePanOffsetAxis(long long& panOffset,
			double aspectRatioMultiplier);

		bool ApplyZoomPan();
		long long ComputeZoomPanAxis(double screenCoord, double screenSize,
			double viewportSize, double viewportScale, double worldSizeDiff);

		bool ApplyMousePan();
		long long ComputeMousePanAxis(double& lastScreenCoord, double currentScreenCoord,
			double screenSize, double viewportSize, double viewportScale, double worldSize);

		bool ApplyZoom();

		bool ApplyPan();
		double ComputePanAxis(long long& pan);

		double ScaleZoom(double zoom);
		double GetMaxPanAtZoom(double zoom);
		double GetVisibleWorldSize(double worldSize,
			double screenSize, double viewportSize);
};
