#version 400

precision highp float;

uniform sampler2D tex;

in vec2 out_TexCoord;
out vec4 fragColor;

void main()
{
	fragColor = texture(tex, out_TexCoord);
}
