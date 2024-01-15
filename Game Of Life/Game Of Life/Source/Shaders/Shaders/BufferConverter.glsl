#version 460 core

layout (local_size_x = 8, local_size_y = 4, local_size_z = 1) in;

uniform int width;
restrict writeonly uniform image2D dataTexture;

layout(std430) restrict readonly buffer dataBuffer
{
	uint Data[];
};

void main()
{
	ivec2 pos = ivec2(gl_GlobalInvocationID.xy);
	uint id = pos.y * width + pos.x;

	float color = float(Data[id]) / 2;
	imageStore(dataTexture, pos, vec4(color, color, color, 1.0));
}
