#include "ScreenDrawer.h"

#include "glad/glad.h"
#include "Settings/Settings.h"
#include "Shaders/Shader/Shader.h"
#include "Shaders/Buffers/Texture/Texture.h"
#include "Shaders/ComputeShader/ComputeShader.h"

ScreenDrawer::ScreenDrawer(int width, int height)
{
	ApplyTranslations();
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

	glGenBuffers(1, &vertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

void ScreenDrawer::Draw()
{
	ApplyTranslations();
	bufferConverter->Execute();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	screenQuad->Activate();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->GetId());

	glBindVertexArray(vertexArrayId);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void ScreenDrawer::ApplyTranslations()
{
	bool updatedZoom = UpdateZoom();
	bool updatedPan = UpdatePan();

	if (updatedZoom || updatedPan)
	{
		lastPanX = Settings::CurrentPanX;
		lastPanY = Settings::CurrentPanY;
		lastZoom = Settings::CurrentZoom;

		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	}
}

bool ScreenDrawer::UpdateZoom()
{
	if (lastZoom == Settings::CurrentZoom)
		return false;

	float zoom = powf(1.2f, Settings::CurrentZoom / 100.0f);
	
	for (int i = 0; i < sizeof(quadVertices) / sizeof(*quadVertices); i += 4)
	{
		quadVertices[i] = initialQuadVertices[i] * zoom;
		quadVertices[i + 1] = initialQuadVertices[i + 1] * zoom;
	}

	return true;
}

bool ScreenDrawer::UpdatePan()
{
	if (lastPanX == Settings::CurrentPanX && lastPanY == Settings::CurrentPanY)
		return false;

	float panX = ComputePanAxis(lastPanX, Settings::CurrentPanX);
	float panY = ComputePanAxis(lastPanY, Settings::CurrentPanY);

	// Apply new vertex coordinates
	for (int i = 2; i < sizeof(quadVertices) / sizeof(*quadVertices); i += 4)
	{
		quadVertices[i] = initialQuadVertices[i] + panX;
		quadVertices[i + 1] = initialQuadVertices[i + 1] + panY;
	}

	return true;
}

float ScreenDrawer::ComputePanAxis(int& lastPan, int& currentPan)
{
	// Calculate pan offset
	int panOffset = currentPan - lastPan;

	// Scale pan offset according to current zoom
	float scale = 1.0f / powf(1.14f, Settings::CurrentZoom / 100.0f);
	int scaledPanOffset = (int)roundf(panOffset * scale);

	// Ensure the scaled pan offset is at least 1
	if (scaledPanOffset == 0 && panOffset != 0) scaledPanOffset = panOffset / abs(panOffset);

	// Add scaled pan offset to last frame's pan
	currentPan = lastPan + scaledPanOffset;

	// Convert pan to vertex coordinates
	return currentPan / 2000000000.0f;
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
