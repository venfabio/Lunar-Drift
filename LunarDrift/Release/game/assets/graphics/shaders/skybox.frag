#version 400

precision highp float;

uniform samplerCube u_CubeMap;

in vec3 out_TexCoord;
out vec4 fragColor;

void main() 
{
	fragColor = texture(u_CubeMap, out_TexCoord);
}
