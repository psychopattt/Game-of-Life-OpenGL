#version 460 core

layout (local_size_x = 8, local_size_y = 4, local_size_z = 1) in;

uniform int width;
uniform int height;

layout(std430) restrict readonly buffer inputBuffer
{
	uint Inputs[];
};

layout(std430) restrict writeonly buffer outputBuffer
{
	uint Outputs[];
};

unsigned int GetNeighborCount(ivec2 pos, uint id)
{
	uint neighborCount = 0;

	for (int y = -1; y < 2; y++)
	{
		for (int x = -1; x < 2; x++)
		{
			int neighborX = int(pos.x) + x;
			int neighborY = int(pos.y) + y;

			if (neighborX < 0 || neighborX >= width || neighborY < 0 || neighborY >= height)
				continue;

			uint neighborId = neighborY * width + neighborX;

			if (neighborId == id)
				continue;

			neighborCount += Inputs[neighborId];

			if (neighborCount > 3)
				return neighborCount;
		}
	}

	return neighborCount;
}

void main()
{
	ivec2 pos = ivec2(gl_GlobalInvocationID.xy);
	uint id = pos.y * width + pos.x;

	uint neighborCount = GetNeighborCount(pos, id);
	Outputs[id] = uint((bool(Inputs[id]) && neighborCount == 2) || neighborCount == 3);
}
