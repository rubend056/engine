
<h1 style="display:flex; justify-content:center">
	<img src="../icon.png" alt="drawing" width="30" style="margin-right:10px;margin-left:10px"/> Triangle Engine Manifesto
</h1>

An engine to understand OpenGL. It should be every college/highschool nerd's fork goto to understand a graphics pipeline. Meaning (Keep it Simple, Stupid) KISS principle is key.

The engine will be closely related to OpenGL, calling it's functions directly using Glad as a linker. Meaning a port to another pipeline like Vulkan or DirectX is not foreseen in the near future.

### Status for features/issues will be **Pending**, **Working**, and **Done**

## Engine
* Initializes all modules SDL2 Window, OpenGL context, Game Assets and ImGui
* Runs engine loop

## Engine Player (pending)
* Initializes all modules from Engine except functionality related to editor, ex (ImGui)

## Libs
### Rendering
* Program
* Shader
* Texture
* VAO
* Mesh
* Attribute

### Components
* Camera
* Source (Audio Source)
* Transform