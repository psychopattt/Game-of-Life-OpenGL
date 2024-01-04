#pragma once

#include <memory>

using std::unique_ptr;

class ScreenDrawer
{
	public:
		ScreenDrawer();
		void Draw();
		~ScreenDrawer();

	private:
		unsigned int vertexArrayId;
		unsigned int vertexBufferId;
		unique_ptr<class Texture> texture;
		unique_ptr<class Shader> screenQuad;
		unique_ptr<class ComputeShader> bufferConverter;

		long long lastPanX = 0;
		long long lastPanY = 0;
		unsigned short lastZoom = 0;

		double quadVertices[16];
		const double initialQuadVertices[16] = {
			-1.0f, 1.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 1.0f, 0.0f
		};

		void GenerateVertexObjects();
		void ApplyTransforms();
		bool UpdateZoom();
		void ApplyMouseZoomPan();
		double ComputeMaxPanAtZoom(unsigned short zoom);
		long long ComputePanOffsetAxis(double screenCoord, double screenSize,
			double viewportSize, double oldZoomMaxPan, double newZoomMaxPan);
		double ScaleZoom(unsigned short zoom);
		bool UpdatePan();
		double ComputePanAxis(long long& lastPan, long long& currentPan, long long& panOffset);
};
