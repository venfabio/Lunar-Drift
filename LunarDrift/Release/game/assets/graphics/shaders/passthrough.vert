#version 400

uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjMatrix;

in vec3 in_Position;
in vec2 in_TexCoord;
in vec4 in_Color;

out vec2 out_TexCoord;
out vec4 out_Color;

void main()
{
	out_TexCoord = in_TexCoord;
	out_Color = in_Color;
	gl_Position = u_ProjMatrix * u_ViewMatrix * vec4(in_Position, 1.0);
}
