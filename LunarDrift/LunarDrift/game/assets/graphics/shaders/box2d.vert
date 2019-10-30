#version 400

uniform mat4 u_ModelMatrix;
uniform mat4 u_ScreenMatrix;


in vec2 in_Position;
in vec2 in_TexCoord;

out vec2 out_TexCoord;

void main()
{
   out_TexCoord = in_TexCoord;
   gl_Position = u_ScreenMatrix * u_ModelMatrix * vec4(in_Position, 0.0, 1.0);
}
