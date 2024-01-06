#include "SimulationDrawer.h"

#include "glad/glad.h"
#include "Interface/Interface.h"
#include "Settings/Settings.h"
#include "Shaders/Shader/Shader.h"
#include "Shaders/Buffers/Texture/Texture.h"
#include "Shaders/ComputeShader/ComputeShader.h"
#include "SimulationTransforms/SimulationTransforms.h"

using std::make_unique;

SimulationDrawer::SimulationDrawer()
{
	GenerateVertexObjects();

	int simWidth = Settings::gui->GetSimWidth();
	int simHeight = Settings::gui->GetSimHeight();
	texture = make_unique<Texture>(simWidth, simHeight);

	screenQuad = make_unique<Shader>("VertexDefault", "FragmentDefault");
	screenQuad->SetInt("dataTexture", 0);

	bufferConverter = make_unique<ComputeShader>("BufferConverter", simWidth, simHeight);
	bufferConverter->SetInt("width", simWidth);

	simTransforms = make_unique<SimulationTransforms>(
		quadVertices, std::size(quadVertices)
	);
}

void SimulationDrawer::GenerateVertexObjects()
{
	glGenVertexArrays(1, &vertexArrayId);
	glBindVertexArray(vertexArrayId);

	glGenBuffers(1, &vertexBufferId);
	UpdateQuadVertexBuffer();

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 4 * sizeof(*quadVertices), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1, 2, GL_DOUBLE, GL_FALSE, 4 * sizeof(*quadVertices),
		(void*)(2 * sizeof(*quadVertices))
	);
}

void SimulationDrawer::UpdateQuadVertexBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_DYNAMIC_DRAW);
}

void SimulationDrawer::Draw()
{
	if (simTransforms->ApplyTransforms())
		UpdateQuadVertexBuffer();

	bufferConverter->Execute();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	screenQuad->Activate();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->GetId());

	glBindVertexArray(vertexArrayId);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

SimulationDrawer::~SimulationDrawer() { }
