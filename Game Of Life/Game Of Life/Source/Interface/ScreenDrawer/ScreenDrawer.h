#pragma once

class ScreenDrawer
{
	public:
		ScreenDrawer(int width, int height);
		void Draw();
		~ScreenDrawer();

	private:
		unsigned int vertexArrayId;
		unsigned int vertexBufferId;
		class Texture* texture;
		class Shader* screenQuad;
		class ComputeShader* bufferConverter;

		int lastZoom = -1;
		int lastPanX = -1;
		int lastPanY = -1;

		float quadVertices[16];
		const float initialQuadVertices[16] = {
			-1.0f, 1.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 1.0f, 0.0f
		};

		void GenerateVertexObjects();
		void ApplyTranslations();
		bool UpdateZoom();
		bool UpdatePan();
		float ComputePanAxis(int& lastPan, int& currentPan);
};
