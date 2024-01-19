#pragma once

class GameOfLifeEditMode
{
	public:
		GameOfLifeEditMode();
		void Update();

	private:
		int lastPixelX = -1;
		int lastPixelY = -1;
		class GameOfLife* gameOfLife;

		bool DrawPixels();
		bool ErasePixels();
		void ModifyMousePathPixels(bool pixelState);
		void SetPixel(int coordX, int coordY, bool state);
};
