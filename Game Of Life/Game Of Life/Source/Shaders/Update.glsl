#version 460 core

layout(local_size_x = 16, local_size_y = 16, local_size_z = 1) in;

uniform ivec2 size;
uniform bool edgeLoop;
uniform int birthRules;
uniform int survivalRules;

layout(std430) restrict readonly buffer inputBuffer {
	uint Inputs[];
};

layout(std430) restrict writeonly buffer outputBuffer {
	uint Outputs[];
};

int GetNeighborId(int neighborX, int neighborY)
{
	if (edgeLoop)
	{
		if (neighborX < 0)
			neighborX = size.x - 1;
		else if (neighborX >= size.x)
			neighborX = 0;

		if (neighborY < 0)
			neighborY = size.y - 1;
		else if (neighborY >= size.y)
			neighborY = 0;
	}
	else if (neighborX < 0 || neighborX >= size.x || neighborY < 0 || neighborY >= size.y)
	{
		return -1;
	}

	return neighborY * size.x + neighborX;
}

uint GetNeighborCount(ivec2 pos, uint id)
{
	uint neighborCount = 0;

	for (int y = -1; y < 2; y++)
	{
		for (int x = -1; x < 2; x++)
		{
			int neighborId = GetNeighborId(pos.x + x, pos.y + y);

			if (neighborId != -1 && neighborId != id)
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

	if (pos.x >= size.x || pos.y >= size.y)
		return;

	uint id = pos.y * size.x + pos.x;
	Outputs[id] = uint(ComputeCellState(pos, id));
}
