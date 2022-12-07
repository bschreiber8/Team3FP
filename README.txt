1. A uniform variable applies to all vertices. The MVP matrix should be uniform because the model, view, and projection matrices are the same regardless of the vertex
2. An attribute variable is specific to each vertex. Vertex position should be an attribute because each vertex has a unique position.
3. A varying variable is a variable that is passed from the vertex shader to the fragment shader. The color should be varying because the color is stored per-vertex and must be transferred to the fragment shader.
4. The glsl code is compiled in the c++ program, so we can edit the glsl code, and rerun the c++ program without needing to recompile the c++ code.
5. Since the color is determined by the vertex position, the object looks like a rainbow
6. This lab did clear up my confusion with glsl and shaders
7. 6
8. Lab writeup was perfect
9. This lab took me 2 hours
10. no