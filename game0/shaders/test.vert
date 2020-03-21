#version 450 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 tex_cord;


layout (location = 0) out vec2 Tex_cord;

void main()
{
	Tex_cord = tex_cord;
    gl_Position = vec4(-pos, 1.0);
}