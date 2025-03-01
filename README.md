# **DirectX 12 Renderer**
## Render engine with DirectX 12 for backend.

Engine with > 30k lines of code that loads .obj files from config.  
**Supports various post-process effects, lighting, reflections and other.**  

_All features are interactible through ImGui UI interface._  
#### You can watch the demo on YouTube: [Link](https://www.youtube.com/watch?v=VI4b0bAk0jk)
## Full list of features:
### Post Processing
- Gaussian blur
- Bilateral blur
- Sobel filter
- Screen space ambient occlusion
### Lighting
- Shadow mapping
- Normal mapping
- Cascade shadows (With Orthographic frustum culling)
- Directional, Spot, Point lights
- BlinnPhong BRDF
- Disney BRDF
### Color
- Gamma correction
- ACES Tone mapping
### Other
- Planar Reflections
- Cube mapping
- Shader reflection
- Shader Hot Reload
- Material editor
- Dynamic Render graph
- Automatic PSO resolve
- Configuration files
- Camera animations
- Debug shaders
- UI settings

## Planned:
- Full support DXR 1.1 and realtime raytracing
- Light mapping
- SSR

## How to Build
1. `git clone --recurse-submodules git@github.com:Cybeaster/Realtime-DX12-Renderer.git`
2. Download [vcpkg](https://github.com/microsoft/vcpkg).
3. Add environment VCPKG_ROOT variable with path to vcpkg directory.
4. Install cmake and Visual Studio 2019/2022.
5. Run setup.cmd
