#pragma once

class Simulation
{
	public:
		virtual void Initialize();
		virtual void Initialize(int width, int height, unsigned int seed = 0) = 0;
		virtual void Restart() = 0;
		virtual void Execute() = 0;
		virtual void Draw() = 0;

		int GetWidth() const;
		int GetHeight() const;
		unsigned int GetSeed() const;

	protected:
		Simulation(int width, int height, unsigned int seed = 0);

		int width;
		int height;
		unsigned int seed;
};
