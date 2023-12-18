#include "Shaders/ComputeShader/ComputeShader.h"
#include "Interface/ScreenDrawer/ScreenDrawer.h"
#include "Interface/CustomInterface/UpdateType.h"
#include "Interface/CustomInterface/CustomInterface.h"
#include "Shaders/Buffers/DualComputeBuffer/DualComputeBuffer.h"

int main()
{
	int width = 1280;
	int height = 720;

	CustomInterface gui(width, height, "Game Of Life");
	ScreenDrawer screenDrawer(width, height);

	unsigned int* bufferData = new unsigned int[width * height] { };
	DualComputeBuffer dualBuffer(bufferData, width * height * sizeof(unsigned int));
	delete[] bufferData;

	ComputeShader gameInitShader("GameOfLifeInit", width, height);
	gameInitShader.SetInt("width", width);
	gameInitShader.Execute();

	ComputeShader gameShader("GameOfLife", width, height);
	gameShader.SetInt("height", height);
	gameShader.SetInt("width", width);

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

	screenDrawer.Destroy();
	dualBuffer.Destroy();
	gui.Destroy();

	return EXIT_SUCCESS;
}
