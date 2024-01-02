#include "Shaders/ComputeShader/ComputeShader.h"
#include "Interface/ScreenDrawer/ScreenDrawer.h"
#include "Interface/CustomInterface/UpdateType.h"
#include "Interface/CustomInterface/CustomInterface.h"
#include "Shaders/Buffers/DualComputeBuffer/DualComputeBuffer.h"

int main()
{
	int uiWidth = 1280;
	int uiHeight = 720;
	int simWidth = 640;
	int simHeight = 360;

	CustomInterface gui(uiWidth, uiHeight, simWidth, simHeight, "Game Of Life");
	ScreenDrawer screenDrawer = ScreenDrawer();

	unsigned int* bufferData = new unsigned int[simWidth * simHeight] { };
	DualComputeBuffer dualBuffer(bufferData, simWidth * simHeight * sizeof(*bufferData));
	delete[] bufferData;

	ComputeShader gameInitShader("GameOfLifeInit", simWidth, simHeight);
	gameInitShader.SetInt("width", simWidth);
	gameInitShader.Execute();

	ComputeShader gameShader("GameOfLife", simWidth, simHeight);
	gameShader.SetInt("height", simHeight);
	gameShader.SetInt("width", simWidth);

	while (!gui.ShouldExit())
	{
		UpdateType updateType = gui.Update();

		if (updateType & Game)
		{
			gameShader.Execute();
			dualBuffer.Swap();
		}

		if (updateType & Interface)
			screenDrawer.Draw();
	}

	return EXIT_SUCCESS;
}
