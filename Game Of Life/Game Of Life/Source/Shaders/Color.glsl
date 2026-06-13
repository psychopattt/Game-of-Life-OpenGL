#version 460 core

layout(local_size_x = 16, local_size_y = 16, local_size_z = 1) in;

uniform ivec2 size;

restrict writeonly uniform image2D texture;
layout(std430) restrict readonly buffer cellsBuffer {
	uint Cells[];
};

void main()
{
	ivec2 position = ivec2(gl_GlobalInvocationID.xy);

	if (position.x >= size.x || position.y >= size.y)
		return;

	uint id = position.y * size.x + position.x;
	float color = float(Cells[id]) * 0.5;
	imageStore(texture, position, vec4(color, color, color, 1.0));
}
