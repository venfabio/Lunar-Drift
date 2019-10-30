#version 400

precision highp float;

uniform sampler2D tex;

in vec2 out_TexCoord;
out vec4 fragColor;

void main()
{
	vec3 color = texture(tex, out_TexCoord).rgb;
	fragColor = vec4(color, 1.0);
}
