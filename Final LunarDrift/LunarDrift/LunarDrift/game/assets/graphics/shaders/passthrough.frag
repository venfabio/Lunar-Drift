#version 400

precision highp float;

in vec4 out_Color;
out vec4 fragColor;

void main()
{
   fragColor = out_Color;
}
