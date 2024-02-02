#version 460 core

layout(local_size_x = 16, local_size_y = 16, local_size_z = 1) in;

uniform int width;
uniform int height;
uniform bool edgeLoop;
uniform int birthRules;
uniform int survivalRules;

layout(std430) restrict readonly buffer inputBuffer
{
	uint Inputs[];
};

layout(std430) restrict writeonly buffer outputBuffer
{
	uint Outputs[];
};

int GetNeighborId(int neighborX, int neighborY)
{
	if (edgeLoop)
	{
		if (neighborX < 0)
			neighborX = width - 1;
		else if (neighborX >= width)
			neighborX = 0;

		if (neighborY < 0)
			neighborY = height - 1;
		else if (neighborY >= height)
			neighborY = 0;
	}
	else if (neighborX < 0 || neighborX >= width || neighborY < 0 || neighborY >= height)
	{
		return -1;
	}

	return neighborY * width + neighborX;
}

unsigned int GetNeighborCount(ivec2 pos, uint id)
{
	uint neighborCount = 0;

	for (int y = -1; y < 2; y++)
	{
		for (int x = -1; x < 2; x++)
		{
			int neighborId = GetNeighborId(pos.x + x, pos.y + y);

			if (neighborId == id || neighborId == -1)
				continue;

			neighborCount += Inputs[neighborId];
		}
	}

	return neighborCount;
}

bool ComputeCellState(ivec2 cellPos, uint cellId)
{
	uint neighborCount = GetNeighborCount(cellPos, cellId);
	int rules = bool(Inputs[cellId]) ? survivalRules : birthRules;

	return bool((1 << neighborCount) & rules);
}

void main()
{
	ivec2 pos = ivec2(gl_GlobalInvocationID.xy);

	if (pos.x >= width || pos.y >= height)
		return;

	uint id = pos.y * width + pos.x;
	Outputs[id] = uint(ComputeCellState(pos, id));
}
