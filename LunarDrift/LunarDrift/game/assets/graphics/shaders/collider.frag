#version 400

precision lowp float;

uniform vec4 u_Color;

out vec4 fragColor;

void main()
{
   fragColor = u_Color;
}
