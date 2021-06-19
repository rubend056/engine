#version 130

// The position attribute for this vertex in local space
in vec3 pos;
// The normal attribute for this vertex in local space
in vec3 norm;
// The texture coordinate for this vertex in local space
in vec2 tex_cord;


// Passing on the texture coordinate
out vec2 Tex_cord;

// The time in the game
uniform double itime;
// Transformation matrix, to convert vertex position in local space to screen coordinates
// which is what this thing will output
uniform mat4 ipmat; 

const float PI = 3.1415926535897932384626433832795;

void main()
{
	Tex_cord = tex_cord;
	
	float tt = float(itime) * PI * float(.0001);
	mat3 test_trans = mat3(
		cos(tt) ,sin(tt),0,  
		-sin(tt),cos(tt),0,  
		0	   ,0	  ,1) * float(1 + sin(tt) * .1);
	vec4 out_pos = 
			ipmat 
			//* mat4(test_trans) 
			* vec4(pos,1);
	
    gl_Position = ipmat * vec4(pos,1);
}