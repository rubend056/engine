#version 460

in vec3 pos_v;
in vec3 tex_c_v;

out vec4 col;

uniform sampler2D tex;

void main()
{
	float v = (pos_v.x+pos_v.y)+1;
	v = v/2;
	col =
	vec4(texture(tex, tex_c_v.xy).xyz, 1);
}
