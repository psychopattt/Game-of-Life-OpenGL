#include "ScreenDrawer.h"

#include <cmath>

#include "glad/glad.h"
#include "Settings/TransformSettings/TransformSettings.h"
#include "Interface/CustomInterface/CustomInterface.h"
#include "Settings/Settings.h"
#include "Shaders/Shader/Shader.h"
#include "Shaders/Buffers/Texture/Texture.h"
#include "Shaders/ComputeShader/ComputeShader.h"

using std::make_unique;

ScreenDrawer::ScreenDrawer()
{
	GenerateVertexObjects();

	int simWidth = Settings::gui->GetSimWidth();
	int simHeight = Settings::gui->GetSimHeight();
	texture = make_unique<Texture>(simWidth, simHeight);

	screenQuad = make_unique<Shader>("VertexDefault", "FragmentDefault");
	screenQuad->SetInt("dataTexture", 0);

	bufferConverter = make_unique<ComputeShader>("BufferConverter", simWidth, simHeight);
	bufferConverter->SetInt("width", simWidth);
}

void ScreenDrawer::GenerateVertexObjects()
{
	std::copy(initialQuadVertices, std::end(initialQuadVertices), quadVertices);

	glGenVertexArrays(1, &vertexArrayId);
	glBindVertexArray(vertexArrayId);

	glGenBuffers(1, &vertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 4 * sizeof(*quadVertices), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1, 2, GL_DOUBLE, GL_FALSE, 4 * sizeof(*quadVertices),
		(void*)(2 * sizeof(*quadVertices))
	);
}

void ScreenDrawer::Draw()
{
	ApplyTransforms();
	bufferConverter->Execute();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	screenQuad->Activate();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->GetId());

	glBindVertexArray(vertexArrayId);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void ScreenDrawer::ApplyTransforms()
{
	bool updatedZoom = UpdateZoom();
	bool updatedPan = UpdatePan();

	if (updatedZoom || updatedPan)
	{
		lastPanX = TransformSettings::PanX;
		lastPanY = TransformSettings::PanY;
		lastZoom = TransformSettings::Zoom;

		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	}
}

bool ScreenDrawer::UpdateZoom()
{
	if (lastZoom == TransformSettings::Zoom)
		return false;

	double scaledZoom = ScaleZoom(TransformSettings::Zoom);
	
	for (int i = 0; i < std::size(quadVertices); i += 4)
	{
		quadVertices[i] = initialQuadVertices[i] * scaledZoom;
		quadVertices[i + 1] = initialQuadVertices[i + 1] * scaledZoom;
	}

	if (TransformSettings::ZoomOnMouse)
		ApplyMouseZoomPan();

	return true;
}

void ScreenDrawer::ApplyMouseZoomPan()
{
	using Settings::gui, TransformSettings::PanOffsetX, TransformSettings::PanOffsetY;

	double oldZoomMaxPan = ComputeMaxPanAtZoom(lastZoom);
	double newZoomMaxPan = ComputeMaxPanAtZoom(TransformSettings::Zoom);
	
	double mousePosX, mousePosY;
	Settings::gui->GetMousePosition(&mousePosX, &mousePosY);

	int viewportWidth, viewportHeight;
	gui->GetViewportSize(viewportWidth, viewportHeight);

	PanOffsetX = ComputePanOffsetAxis(mousePosX, gui->GetWidth(), viewportWidth, oldZoomMaxPan, newZoomMaxPan);
	PanOffsetY = -ComputePanOffsetAxis(mousePosY, gui->GetHeight(), viewportHeight, oldZoomMaxPan, newZoomMaxPan);
}

double ScreenDrawer::ComputeMaxPanAtZoom(unsigned short zoom)
{
	// Calculate pan scale at specified zoom
	double panScale = 1.0 / ScaleZoom(zoom);

	// Multipy max pan without zoom by pan scale
	return TransformSettings::MaxPan * panScale;
}

long long ScreenDrawer::ComputePanOffsetAxis(double screenCoord, double screenSize,
	double viewportSize, double oldWorldSize, double newWorldSize)
{
	// Get screen coord relative to center [-1, 1] from absolute screen coord
	double screenCoordRelativeCenter = 2.0 * screenCoord / screenSize - 1.0;

	// Calculate visible portion of the world size
	double visibleWorldSizeRatio = screenSize / viewportSize;
	double oldVisibleWorldSize = oldWorldSize * visibleWorldSizeRatio;
	double newVisibleWorldSize = newWorldSize * visibleWorldSizeRatio;

	// Map visible world size to [-1, 1] by dividing by 2
	// Multiply relative screen coord by world size to get world coord
	double oldWorldCoord = oldVisibleWorldSize / 2.0 * screenCoordRelativeCenter;
	double newWorldCoord = newVisibleWorldSize / 2.0 * screenCoordRelativeCenter;

	// Substract new coord from old coord to get world coord offset
	return llround(oldWorldCoord - newWorldCoord);
}

double ScreenDrawer::ScaleZoom(unsigned short zoom)
{
	return pow(1.2, static_cast<double>(zoom) / TransformSettings::FastMultiplier);
}

bool ScreenDrawer::UpdatePan()
{
	using TransformSettings::PanX, TransformSettings::PanY,
		TransformSettings::PanOffsetX, TransformSettings::PanOffsetY;

	if (lastPanX == PanX && lastPanY == PanY && PanOffsetX == 0 && PanOffsetY == 0)
		return false;

	double panX = ComputePanAxis(lastPanX, PanX, PanOffsetX);
	double panY = ComputePanAxis(lastPanY, PanY, PanOffsetY);

	// Apply new vertex coordinates
	for (int i = 2; i < std::size(quadVertices); i += 4)
	{
		quadVertices[i] = initialQuadVertices[i] + panX;
		quadVertices[i + 1] = initialQuadVertices[i + 1] + panY;
	}

	return true;
}

double ScreenDrawer::ComputePanAxis(long long& lastPan, long long& currentPan, long long& panOffset)
{
	using TransformSettings::MaxPan, TransformSettings::Zoom, TransformSettings::FastMultiplier;

	// Calculate pan difference
	long long panDiff = currentPan - lastPan;

	// Scale pan difference according to current zoom
	double scale = 1.0 / pow(1.14, static_cast<double>(Zoom) / FastMultiplier);
	long long scaledPanOffset = llround(panDiff * scale);

	// Ensure the scaled pan difference is at least 1
	if (scaledPanOffset == 0 && panDiff != 0)
		scaledPanOffset = panDiff / abs(panDiff);

	// Add scaled pan difference to last frame's pan
	currentPan = lastPan + scaledPanOffset;

	// Add pan offset from mouse zoom
	currentPan += panOffset;
	panOffset = 0;

	// Loop pan to opposite side if min or max is reached
	if (currentPan < -MaxPan || currentPan > MaxPan)
		currentPan -= currentPan / abs(currentPan) * MaxPan * 2;

	// Convert pan to vertex coordinates
	return static_cast<double>(currentPan) / MaxPan;
}

ScreenDrawer::~ScreenDrawer() { }
