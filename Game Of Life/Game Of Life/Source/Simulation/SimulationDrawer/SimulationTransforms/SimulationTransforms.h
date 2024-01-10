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

		bool ApplyZoom();
		void ApplyMouseZoomPan();
		double ComputeMaxPanAtZoom(unsigned short zoom);
		long long ComputePanOffsetAxis(double screenCoord, double screenSize,
			double viewportSize, double viewportScale, double worldSizeDiff);
		double ScaleZoom(unsigned short zoom);
		bool ApplyPan();
		double ComputePanAxis(long long& lastPan, long long& currentPan,
			double aspectRatioMultiplier, long long& panOffset);
};
