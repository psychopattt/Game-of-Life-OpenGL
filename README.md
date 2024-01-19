# Game of Life
[Conway's Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life) implemented with GLFW, OpenGL and Dear ImGui

## Features
- Simulation
  - Resize
  - Edit mode (E)
    - Draw cells (left click)
    - Erase cells (right click)
  - Control target FPS
  - Step to next frame (F)
  - Toggle edge loop
  - Restart (F5)
- Interface
  - Pan and zoom
    - Zoom with mouse wheel
    - Pan with WASD, arrows or mouse drag
  - Show metrics window
    - Simulation FPS and frametimes
    - Interface FPS and frametimes
  - Toggle fullscreen (F11)

## Requirements
- An OpenGL 4.6 capable GPU

## How to use
Windows
1. Download the [latest release](https://github.com/psychopattt/Game-of-Life-OpenGL/releases/latest)
2. Run the executable

Linux or MacOS
1. Download the project files
3. Replace the included Windows GLFW binaries
   1. Get the [GLFW binaries](https://www.glfw.org/download.html) for your OS
   2. Replace the files in "Libraries/include/GLFW"
   3. Replace the files in "Libraries/lib"
4. Compile and run the project

## Credits
- [GLFW](https://www.glfw.org)
- [GLAD](https://github.com/Dav1dde/glad)
- [Dear ImGui](https://github.com/ocornut/imgui)
- [Proggy Fonts](https://github.com/bluescan/proggyfonts)
