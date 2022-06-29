#version 330

layout (triangles) in;
layout (line_strip /*for lines, use "points" for points*/, max_vertices=3) out;

in vec2 texcoords_pass[]; //Texcoords from Vertex Shader
in vec3 normals_pass[]; //Normals from Vertex Shader

out vec3 normals; //Normals for Fragment Shader
out vec2 texcoords; //Texcoords for Fragment Shader

void main(void)
{
    int i;
    for (i = 0; i < gl_in.length(); i++)
    {
        texcoords=texcoords_pass[i]; //Pass through
        normals=normals_pass[i]; //Pass through
        gl_Position = gl_in[i].gl_Position; //Pass through
        EmitVertex();
    }
    EndPrimitive();
}