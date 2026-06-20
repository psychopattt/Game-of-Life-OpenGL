#version 460 core

layout(local_size_x = 16, local_size_y = 16, local_size_z = 1) in;

uniform ivec2 size;
uniform ivec2 lastPosition;
uniform ivec2 currentPosition;
uniform bool edgeLoop;
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

void UpdateCell(ivec2 lineStart, ivec2 lineEnd, ivec2 position)
{
    if (LineDistance(lineStart, lineEnd, position) < 0.5)
        Cells[position.y * size.x + position.x] = drawing ? 1 : 0;
}

void HandleDrawLooping(ivec2 position)
{
    for (int y = -1; y < 2; y++)
    {
        for (int x = -1; x < 2; x++)
        {
            ivec2 offset = ivec2(x, y) * size;
            ivec2 offsetLineStart = lastPosition + offset;
            ivec2 offsetLineEnd = currentPosition + offset;
            UpdateCell(offsetLineStart, offsetLineEnd, position);
        }
    }
}

void main()
{
    ivec2 position = ivec2(gl_GlobalInvocationID.xy);

    if (position.x >= size.x || position.y >= size.y)
        return;

    if (edgeLoop) {
        HandleDrawLooping(position);
    } else {
        UpdateCell(lastPosition, currentPosition, position);
    }
}
