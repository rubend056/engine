#version 460

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 tex_c;
layout (location = 2) in vec3 nm;
uniform double itime;

out vec3 pos_v;
out vec3 tex_c_v;

void main()
{
	vec3 p = pos * (1+.1*cos(float(itime)/400));
	gl_Position = vec4(p, 1.0);
	pos_v = p;
	tex_c_v = tex_c;
}
