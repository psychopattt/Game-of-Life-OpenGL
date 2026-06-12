#version 460 core

layout(local_size_x = 16, local_size_y = 16, local_size_z = 1) in;

uniform ivec2 size;
uniform ivec2 lastPosition;
uniform ivec2 currentPosition;
uniform bool drawing;

layout(std430) restrict writeonly buffer cellsBuffer {
	uint Cells[];
};

float LineDistance(vec2 lineStart, vec2 lineEnd, vec2 point)
{
    vec2 pointToStart = point - lineStart;
    vec2 endToStart = lineEnd - lineStart;
    float heightOnLine = clamp(dot(pointToStart, endToStart) /
        dot(endToStart, endToStart), 0, 1);

    return length(pointToStart - endToStart * heightOnLine);
}

void main()
{
    ivec2 position = ivec2(gl_GlobalInvocationID.xy);

    if (position.x >= size.x || position.y >= size.y)
        return;

    if (LineDistance(lastPosition, currentPosition, position) < 0.5)
        Cells[position.y * size.x + position.x] = drawing ? 1 : 0;
}
