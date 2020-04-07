#version 130

in vec3 pos;
in vec3 norm;
in vec2 tex_cord;


out vec2 Tex_cord;

void main()
{
	Tex_cord = tex_cord;
    gl_Position = vec4(-pos, 1.0);
}