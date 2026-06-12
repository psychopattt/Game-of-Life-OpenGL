#version 460 core

layout(local_size_x = 16, local_size_y = 16, local_size_z = 1) in;

uniform int width;
uniform int height;

restrict writeonly uniform image2D texture;
layout(std430) restrict readonly buffer cellsBuffer {
	uint Cells[];
};

void main()
{
	ivec2 pos = ivec2(gl_GlobalInvocationID.xy);

	if (pos.x >= width || pos.y >= height)
		return;

	uint id = pos.y * width + pos.x;
	float color = float(Cells[id]) * 0.5;
	imageStore(texture, pos, vec4(color, color, color, 1.0));
}
