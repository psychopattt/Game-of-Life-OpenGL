#version 460 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 vertexTextureCoords;
out vec2 fragmentTextureCoords;

void main()
{
	gl_Position = position;
	fragmentTextureCoords = vertexTextureCoords;
}
