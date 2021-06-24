#version 130

in vec2 Tex_cord;

out vec4 outColor;

uniform vec3 color_uniform;
uniform sampler2D tex;

void main()
{
	outColor = 
	texture(tex, Tex_cord) * 
    vec4(color_uniform , 1);
}
