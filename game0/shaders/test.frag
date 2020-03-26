#version 450 core

layout (location = 0) in vec2 Tex_cord;

layout (location = 0) out vec4 outColor;

uniform vec3 color_uniform;
uniform sampler2D tex;

void main()
{
	outColor = 
	texture(tex, Tex_cord) *
	vec4(color_uniform, 1) + vec4(.1);
}
