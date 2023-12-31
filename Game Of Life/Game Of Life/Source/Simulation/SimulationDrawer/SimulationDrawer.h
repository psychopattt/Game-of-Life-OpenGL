#pragma once

#include <memory>

using std::unique_ptr;

class SimulationDrawer
{
	public:
		SimulationDrawer(int width, int height);
		void Draw(unsigned int dataBufferBinding);
		~SimulationDrawer();

	private:
		unsigned int vertexArrayId;
		unsigned int vertexBufferId;
		unique_ptr<class Texture> texture;
		unique_ptr<class Shader> screenQuad;
		unique_ptr<class ComputeShader> bufferConverter;
		unique_ptr<class SimulationTransforms> simTransforms;

		double quadVertices[16] = {
			-1.0f, 1.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 1.0f, 0.0f
		};

		void GenerateVertexObjects();
		void UpdateQuadVertexBuffer();
};
