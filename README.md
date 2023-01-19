# CookieEngine
Wojciech Czerski - PJATK - Engineering Thesis

## How to install?
Create folder where you want to keep project, for example 
> root\MyProject

Create Dependencies folder where you keep all additional libraries.
> root\MyProject\Dependencies

Move there downloaded libraries ImGui, OGRE-next.

### OGRE

For OGRE and it's dependencies do build with this tutorial step-by-step 

https://ogrecave.github.io/ogre-next/api/latest/_setting_up_ogre_windows.html#BuildingOgreWindows

Include SDL2 build library to Release folder in bin.
To run DirectX add amd_ags_x64.dll
Add Ogre Atmosphere.dll to bin\release

Clear %AppData%/CookiEngine to clear run config

### ImGui

Works out-of-the-box. If not, please add CMakeLists with ImGui lib definition to Dependencies folder.
Then include it in main CMakeLists.

### Special thanks
For creating imgui integration files and implementation:
https://github.com/edherbert/ogre-next-imgui

### Execution build

After proper installation hit compile on release version. 
For more info / errors fix ping me in sections below.

## How it works

### What it includes


