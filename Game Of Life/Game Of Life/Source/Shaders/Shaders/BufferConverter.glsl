#version 460 core

layout (local_size_x = 8, local_size_y = 4, local_size_z = 1) in;

uniform int width;
layout(rgba32f, binding = 0) writeonly uniform image2D texture;

layout(std430, binding = 2) restrict readonly buffer dataBuffer
{
    uint Data[];
};

void main()
{
    ivec2 pos = ivec2(gl_GlobalInvocationID.xy);
    uint id = pos.y * width + pos.x;

    float color = float(Data[id]) / 2;
    imageStore(texture, pos, vec4(color, color, color, 1.0));
}
