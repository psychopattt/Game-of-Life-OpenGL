#version 460 core

layout (local_size_x = 8, local_size_y = 4, local_size_z = 1) in;

uniform int width;

layout(std430, binding = 2) restrict writeonly buffer dataBuffer
{
	uint Data[];
};

uint Random(uint state)
{
	state ^= 2747636419;
	state *= 2654435769;
	state ^= state >> 16;
	state *= 2654435769;
	state ^= state >> 16;
	state *= 2654435769;
	return state;
}

void main()
{
	ivec2 pos = ivec2(gl_GlobalInvocationID.xy);
	uint id = pos.y * width + pos.x;
	Data[id] = Random(id) % 2;
}
