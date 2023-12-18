#pragma once

class ScreenDrawer
{
	public:
		ScreenDrawer(int width, int height);
		void Draw() const;
		void Destroy() const;

	private:
		unsigned int vertexArrayId;
		class Texture* texture;
		class Shader* screenQuad;
		class ComputeShader* bufferConverter;

		void GenerateVertexObjects();
};
