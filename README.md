## ***EGL***
### OpenGL ES needs a rendering context and drawing surface, so we need EGL to:
1. Query displays and initialize them.
2. Create a rendering surface.
3. Create a rendering context.
    
## ***OpenGL 3.0 view processing flow***
### 1. Generate vertex shader
- Input posisition and color, use matrix to convert position from model space to device space, output vertex and vertex color.
### 2. Primitive assembly
- Clip and culling the view of vertex shader which out of screen or in face background.
### 3. Rasterization
- Convert primitives into a set of two-dimensional (like $`(x_w, y_w)`$) fragments to fit fragment shader.
### 4. Generate Fragment shader
- Input vertex color, output fragment color.
### 5. Per-fragment operations
- Fragment data -> Pixel ownership test -> Scissor test -> Stencil and depth tests -> Blending -> Dithering
