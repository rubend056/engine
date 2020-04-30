#version 130

in vec3 pos;
in vec3 norm;
in vec2 tex_cord;

out vec2 Tex_cord;

uniform double time;

const float PI = 3.1415926535897932384626433832795;

void main()
{
	Tex_cord = tex_cord;
	
	float tt = float(time) * PI * float(.001);
	mat3 trans = mat3(cos(tt),sin(tt),0,  -sin(tt),cos(tt),0,  0,0,1) * float(1 + sin(tt) * .5);
	vec3 out_pos = trans * pos;
	
    gl_Position = vec4(out_pos, 1.0);
}