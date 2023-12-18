// Generated code

#if !DEBUG
#include "ShaderProvider.h"
#include <string>
const string ShaderProvider::UnpackCode(const uint32_t& h) {
switch (h) {
case 1627215959: return "#version 460 core\n\nlayout (local_size_x = 8, local_size_y = 4, local_size_z = 1) in;\n\nuniform int width;\nlayout(rgba32f, binding = 0) writeonly uniform image2D texture;\n\nlayout(std430, binding = 2) restrict readonly buffer dataBuffer\n{\n    uint Data[];\n};\n\nvoid main()\n{\n    ivec2 pos = ivec2(gl_GlobalInvocationID.xy);\n    uint id = pos.y * width + pos.x;\n\n    float color = float(Data[id]) / 2;\n    imageStore(texture, pos, vec4(color, color, color, 1.0));\n}\n";
case 1939412094: return "#version 460 core\n\nin vec2 fragmentTextureCoords;\nuniform sampler2D dataTexture;\nout vec4 color;\n\nvoid main()\n{\n    color = texture(dataTexture, fragmentTextureCoords);\n}\n";
case 1263596596: return "#version 460 core\n\nlayout (local_size_x = 8, local_size_y = 4, local_size_z = 1) in;\n\nuniform int width;\nuniform int height;\n\nlayout(std430, binding = 2) restrict readonly buffer inputBuffer\n{\n    uint Inputs[];\n};\n\nlayout(std430, binding = 3) restrict writeonly buffer outputBuffer\n{\n    uint Outputs[];\n};\n\nunsigned int GetNeighborCount(ivec2 pos, uint id)\n{\n    uint neighborCount = 0;\n\n    for (int y = -1; y < 2; y++)\n    {\n        for (int x = -1; x < 2; x++)\n        {\n            int neighborX = int(pos.x) + x;\n            int neighborY = int(pos.y) + y;\n\n            if (neighborX < 0 || neighborX >= width || neighborY < 0 || neighborY >= height)\n                continue;\n\n            uint neighborId = neighborY * width + neighborX;\n\n            if (neighborId == id)\n                continue;\n\n            neighborCount += Inputs[neighborId];\n\n            if (neighborCount > 3)\n                return neighborCount;\n        }\n    }\n\n    return neighborCount;\n}\n\nvoid main()\n{\n    ivec2 pos = ivec2(gl_GlobalInvocationID.xy);\n    uint id = pos.y * width + pos.x;\n\n    uint neighborCount = GetNeighborCount(pos, id);\n    Outputs[id] = uint((bool(Inputs[id]) && neighborCount == 2) || neighborCount == 3);\n}\n";
case 2357429320: return "#version 460 core\n\nlayout (local_size_x = 8, local_size_y = 4, local_size_z = 1) in;\n\nuniform int width;\n\nlayout(std430, binding = 2) restrict writeonly buffer dataBuffer\n{\n    uint Data[];\n};\n\nuint Random(uint state)\n{\n    state ^= 2747636419;\n    state *= 2654435769;\n    state ^= state >> 16;\n    state *= 2654435769;\n    state ^= state >> 16;\n    state *= 2654435769;\n    return state;\n}\n\nvoid main()\n{\n    ivec2 pos = ivec2(gl_GlobalInvocationID.xy);\n    uint id = pos.y * width + pos.x;\n    Data[id] = Random(id) % 2;\n}\n";
case 776390504: return "#version 460 core\n\nlayout (location = 0) in vec4 position;\nlayout (location = 1) in vec2 vertexTextureCoords;\nout vec2 fragmentTextureCoords;\n\nvoid main()\n{\n    gl_Position = position;\n    fragmentTextureCoords = vertexTextureCoords;\n}\n";
default: return ""; }}
#endif