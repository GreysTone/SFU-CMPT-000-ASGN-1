#version 150

in vec4 vPosition;
in vec4 vColor;
out vec4 color;

uniform int xsize;
uniform int ysize;
uniform int zsize;
uniform mat4 MVP;

void main() 
{
//	mat4 scale = mat4(
//	    2.0/xsize,      0.0,        0.0,        0.0,
//		0.0,            2.0/ysize, 	0.0,        0.0,
//		0.0, 		    0.0, 		2.0/zsize,  0.0,
//		0.0, 		    0.0, 		0.0,        1.0);

	// First, center the image by translating each vertex by half of the original window size
	// Then, multiply by the scale matrix to maintain size after the window is reshaped
//	vec4 newPos = vPosition + vec4(-200, -360, 0, 0);
	//gl_Position = scale * newPos;
	vec4 newPos = vPosition + vec4(-200, -660, 0, 0);
    gl_Position = MVP * newPos;

	color = vColor;	
}
