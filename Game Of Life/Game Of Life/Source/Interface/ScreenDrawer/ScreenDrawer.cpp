#include "ScreenDrawer.h"

#include "glad/glad.h"
#include "Shaders/Shader/Shader.h"
#include "Shaders/Buffers/Texture/Texture.h"
#include "Shaders/ComputeShader/ComputeShader.h"

static const float quadVertices[] = {
	-1.0f,  1.0f, 0.0f, 1.0f,
	-1.0f, -1.0f, 0.0f, 0.0f,
	 1.0f,  1.0f, 1.0f, 1.0f,
	 1.0f, -1.0f, 1.0f, 0.0f,
};

ScreenDrawer::ScreenDrawer(int width, int height)
{
	GenerateVertexObjects();
	texture = new Texture(width, height);

	screenQuad = new Shader("VertexDefault", "FragmentDefault");
	screenQuad->SetInt("dataTexture", 0);

	bufferConverter = new ComputeShader("BufferConverter", width, height);
	bufferConverter->SetInt("width", width);
}

void ScreenDrawer::GenerateVertexObjects()
{
	glGenVertexArrays(1, &vertexArrayId);
	glBindVertexArray(vertexArrayId);

	unsigned int vertexBufferId;
	glGenBuffers(1, &vertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

void ScreenDrawer::Draw() const
{
	bufferConverter->Execute();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	screenQuad->Activate();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->GetId());

	glBindVertexArray(vertexArrayId);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void ScreenDrawer::Destroy() const
{
	texture->Destroy();
	screenQuad->Destroy();
	bufferConverter->Destroy();

	delete texture;
	delete screenQuad;
	delete bufferConverter;
}
