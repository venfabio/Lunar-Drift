#version 400

uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjMatrix;
uniform mat4 u_ModelMatrix;

in vec3 in_Position;

void main()
{
	gl_Position = u_ProjMatrix * u_ViewMatrix * u_ModelMatrix * vec4(in_Position, 1.0);
}
