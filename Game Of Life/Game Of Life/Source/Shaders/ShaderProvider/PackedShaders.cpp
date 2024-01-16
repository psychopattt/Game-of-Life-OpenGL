// Generated code

#if !DEBUG
#include "ShaderProvider.h"
const string ShaderProvider::UnpackCode(const uint32_t& h) {
switch (h) {
case 1627215959: return "#version 460 core\nlayout(local_size_x=8, local_size_y=4, local_size_z=1) in;uniform int width;restrict writeonly uniform image2D dataTexture;layout(std430) restrict readonly buffer dataBuffer\n{\nuint Data[];};void main()\n{\nivec2 pos=ivec2(gl_GlobalInvocationID.xy); uint id=pos.y * width + pos.x; float color=float(Data[id]) / 2; imageStore(dataTexture, pos, vec4(color, color, color, 1.0));}\n";
case 1939412094: return "#version 460 core\nin vec2 fragmentTextureCoords;uniform sampler2D dataTexture;out vec4 color;void main()\n{\ncolor=texture(dataTexture, fragmentTextureCoords);}\n";
case 1263596596: return "#version 460 core\nlayout(local_size_x=8, local_size_y=4, local_size_z=1) in;uniform int width;uniform int height;layout(std430) restrict readonly buffer inputBuffer\n{\nuint Inputs[];};layout(std430) restrict writeonly buffer outputBuffer\n{\nuint Outputs[];};unsigned int GetNeighborCount(ivec2 pos, uint id)\n{\nuint neighborCount=0; for (int y=-1; y < 2; y++)\n{\nfor (int x=-1; x < 2; x++)\n{\nint neighborX=pos.x + x; int neighborY=pos.y + y; if (neighborX < 0 || neighborX >= width || neighborY < 0 || neighborY >= height)\ncontinue; uint neighborId=neighborY * width + neighborX; if (neighborId == id)\ncontinue; neighborCount += Inputs[neighborId]; if (neighborCount > 3)\nreturn neighborCount; }\n}\nreturn neighborCount;}\nvoid main()\n{\nivec2 pos=ivec2(gl_GlobalInvocationID.xy); uint id=pos.y * width + pos.x; uint neighborCount=GetNeighborCount(pos, id); Outputs[id]=uint((bool(Inputs[id]) && neighborCount == 2) || neighborCount == 3);}\n";
case 2357429320: return "#version 460 core\nlayout(local_size_x=8, local_size_y=4, local_size_z=1) in;uniform int width;uniform uint seed;layout(std430) restrict writeonly buffer dataBuffer\n{\nuint Data[];};uint Random(uint state)\n{\nstate ^= 2747636419; state *= 2654435769; state ^= state >> 16; state *= seed + 1; state *= 2654435769; state ^= state >> 16; state *= 2654435769; return state;}\nvoid main()\n{\nivec2 pos=ivec2(gl_GlobalInvocationID.xy); uint id=pos.y * width + pos.x; Data[id]=Random(id) % 2;}\n";
case 776390504: return "#version 460 core\nin vec4 position;in vec2 vertexTextureCoords;out vec2 fragmentTextureCoords;void main()\n{\ngl_Position=position; fragmentTextureCoords=vertexTextureCoords;}\n";
default: return ""; }}
#endif
