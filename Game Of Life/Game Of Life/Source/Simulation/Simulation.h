#pragma once

class Simulation
{
	public:
		virtual void Initialize()
		{
			Initialize(width, height, seed);
		}

		virtual void Initialize(int width, int height, unsigned int seed = 0) = 0;
		virtual void Restart() = 0;
		virtual void Execute() = 0;
		virtual void Draw() = 0;

		int GetWidth() const { return width; }
		int GetHeight() const { return height; }
		unsigned int GetSeed() const { return seed; }

	protected:
		Simulation(int width, int height, unsigned int seed = 0) :
			width(width), height(height), seed(seed) { }

		int width;
		int height;
		unsigned int seed;
};
