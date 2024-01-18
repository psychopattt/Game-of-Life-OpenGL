#pragma once

class GameOfLifeEditMode
{
	public:
		void Update();

	private:
		void DrawPixels();
		void ErasePixels();
		void EditPixels(bool pixelState);
		int ComputeMousePixelIndex();
};
