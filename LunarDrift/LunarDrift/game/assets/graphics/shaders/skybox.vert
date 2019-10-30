#version 400

uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjMatrix;

in vec3 in_Position;
out vec3 out_TexCoord;
 
void main() 
{
	// Bottom row of row matrix is the aggregated translation;
    // consequently, unsetting it retains the rotation we need
    // without the annoying positional stuff! :D
    // Matrix maths is fun! <3
	mat4 viewMatrix = u_ViewMatrix;
    viewMatrix[3][0] = 0;
    viewMatrix[3][1] = 0;
    viewMatrix[3][2] = 0;

	out_TexCoord = in_Position;

	gl_Position = u_ProjMatrix * viewMatrix * vec4(in_Position, 1.0);
}